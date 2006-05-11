// $Id: xpoilflt.cpp 836 2006-04-18 16:06:15Z gerry $
/* @@tag:xara-cn@@ DO NOT MODIFY THIS LINE
================================XARAHEADERSTART===========================
 
               Xara LX, a vector drawing and manipulation program.
                    Copyright (C) 1993-2006 Xara Group Ltd.
       Copyright on certain contributions may be held in joint with their
              respective authors. See AUTHORS file for details.

LICENSE TO USE AND MODIFY SOFTWARE
----------------------------------

This file is part of Xara LX.

Xara LX is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as published
by the Free Software Foundation.

Xara LX and its component source files are distributed in the hope
that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with Xara LX (see the file GPL in the root directory of the
distribution); if not, write to the Free Software Foundation, Inc., 51
Franklin St, Fifth Floor, Boston, MA  02110-1301 USA


ADDITIONAL RIGHTS
-----------------

Conditional upon your continuing compliance with the GNU General Public
License described above, Xara Group Ltd grants to you certain additional
rights. 

The additional rights are to use, modify, and distribute the software
together with the wxWidgets library, the wxXtra library, and the "CDraw"
library and any other such library that any version of Xara LX relased
by Xara Group Ltd requires in order to compile and execute, including
the static linking of that library to XaraLX. In the case of the
"CDraw" library, you may satisfy obligation under the GNU General Public
License to provide source code by providing a binary copy of the library
concerned and a copy of the license accompanying it.

Nothing in this section restricts any of the rights you have under
the GNU General Public License.


SCOPE OF LICENSE
----------------

This license applies to this program (XaraLX) and its constituent source
files only, and does not necessarily apply to other Xara products which may
in part share the same code base, and are subject to their own licensing
terms.

This license does not apply to files in the wxXtra directory, which
are built into a separate library, and are subject to the wxWindows
license contained within that directory in the file "WXXTRA-LICENSE".

This license does not apply to the binary libraries (if any) within
the "libs" directory, which are subject to a separate license contained
within that directory in the file "LIBS-LICENSE".


ARRANGEMENTS FOR CONTRIBUTION OF MODIFICATIONS
----------------------------------------------

Subject to the terms of the GNU Public License (see above), you are
free to do whatever you like with your modifications. However, you may
(at your option) wish contribute them to Xara's source tree. You can
find details of how to do this at:
  http://www.xaraxtreme.org/developers/

Prior to contributing your modifications, you will need to complete our
contributor agreement. This can be found at:
  http://www.xaraxtreme.org/developers/contribute/

Please note that Xara will not accept modifications which modify any of
the text between the start and end of this header (marked
XARAHEADERSTART and XARAHEADEREND).


MARKS
-----

Xara, Xara LX, Xara X, Xara X/Xtreme, Xara Xtreme, the Xtreme and Xara
designs are registered or unregistered trademarks, design-marks, and/or
service marks of Xara Group Ltd. All rights in these marks are reserved.


      Xara Group Ltd, Gaddesden Place, Hemel Hempstead, HP2 6EX, UK.
                        http://www.xara.com/

=================================XARAHEADEREND============================
 */

// The module that controls the OIL side of import/export filters.


#include "camtypes.h"

#include "camprocess.h"



CamProcess::CamProcess(CCLexFile* pInFile, CCLexFile* pOutFile)
{
	m_bDead = true;
	m_ReturnCode = -1;
	m_pInFile = pInFile;
	m_pOutFile = pOutFile;
	m_BytesIn = 0;
	m_BytesOut = 0;
}

CamProcess::~CamProcess()
{
	if (!m_bDead)
	{
		TRACEUSER("Gerry", _T("Process not dead in ~CamProcess"));
	}
}


INT32 CamProcess::Execute(const wxString& cmd)
{
	// We're now running
	m_bDead = false;

	// Make sure redirection happens
	Redirect();

	if (!wxExecute(cmd, wxEXEC_ASYNC, this))
	{
		// Report problem
		m_bDead = true;
		return 123;
	}

	BYTE ReadBuffer[4096];
	size_t ReadBytes = 0;
	BYTE* ReadPtr = NULL;
	bool bMoreInput = true;
	size_t InFileLeft = 0;
	if (m_pInFile)
	{
		InFileLeft = m_pInFile->Size();
		TRACEUSER("Gerry", _T("InFileSize = %d"), InFileLeft);
	}

	// Loop until m_bDead is true

	while (!m_bDead)
	{
		// Call the virtual function to process any output on stderr
		ProcessStdErr();

		// If we have an output file
		if (m_pOutFile)
		{
			// If there is output from the process
			if (IsInputAvailable())
			{
				// Copy the data to the file

				size_t NumRead = 4096;
				BYTE Buffer[4096];

				while (NumRead > 0)
				{
					// Read a buffer full
					GetInputStream()->Read(Buffer, 4096);

					NumRead = GetInputStream()->LastRead();

					// Write the buffer to the file
					if (NumRead > 0)
					{
						m_pOutFile->write(Buffer, NumRead);
					}
				}
			}
		}
		else
		{
			// Call the virtual function to process the output
			ProcessStdOut();
		}

		// If we have an input file
		if (m_pInFile)
		{
			// Copy some data to the process
			while (bMoreInput)
			{
				// If there is nothing in the buffer
				if (ReadBytes == 0)
				{
					ReadBytes = 4096;
					if (ReadBytes > InFileLeft)
						ReadBytes = InFileLeft;

					if (ReadBytes > 0)
					{
						// Read a buffer full
						TRACEUSER("Gerry", _T("Reading %d"), ReadBytes);
						m_pInFile->read(ReadBuffer, ReadBytes);

						InFileLeft -= ReadBytes;
						ReadPtr = ReadBuffer;
					}
				}

				// If there is something in the buffer
				if (ReadBytes > 0)
				{
					TRACEUSER("Gerry", _T("Buffer contains %d"), ReadBytes);
					// Try to write it to the process
					GetOutputStream()->Write(ReadPtr, ReadBytes);

					size_t Done = GetOutputStream()->LastWrite();
					TRACEUSER("Gerry", _T("Written %d"), Done);
					// If we couldn't write it all
					if (Done < ReadBytes)
					{
						// Update the buffer pointer and size
						ReadBytes -= Done;
						ReadPtr += Done;
						break;				// go and process the other streams
					}
					ReadBytes = 0;
				}
				else
				{
					// Indicate there is no more stdin
					TRACEUSER("Gerry", _T("Buffer is empty - closing"));
					CloseOutput();
					bMoreInput = false;
				}
			}
		}
		else
		{
			// Call the virtual function to process the input
			ProcessStdIn();
		}

		wxYield();
	}

	TRACEUSER("Gerry", _T("Exiting with %d"), m_ReturnCode);
	return m_ReturnCode;
}

void CamProcess::ProcessStdIn()
{
	// Do nothing in here
}


void CamProcess::ProcessStdOut()
{
	if (IsInputAvailable())
	{
		wxTextInputStream tis(*GetInputStream());

		// This assumes that the output is always line buffered
		while (!GetInputStream()->Eof())
		{
			wxString line;
			line << tis.ReadLine();
			TRACEUSER("Gerry", _T("(stdout):%s"), line.c_str());
		}
	}

}


void CamProcess::ProcessStdErr()
{
	if (IsErrorAvailable())
	{
		wxTextInputStream tis(*GetErrorStream());

		// This assumes that the output is always line buffered
		while (!GetErrorStream()->Eof())
		{
			wxString line;
			line << tis.ReadLine();
			TRACEUSER("Gerry", _T("(stderr):%s"), line.c_str());
		}
	}
}


void CamProcess::OnTerminate(int pid, int status)
{
	TRACEUSER("Gerry", _T("CamProcess::OnTerminate pid = %d  status = %d"), pid, status);
	m_bDead = true;
	m_ReturnCode = status;

	// Process anything remaining on stderr and stdout
	// If we have an output file
	if (m_pOutFile)
	{
		// If there is output from the process
		if (IsInputAvailable())
		{
			// Copy the data to the file
			size_t NumRead = 4096;
			BYTE Buffer[4096];

			while (NumRead > 0)
			{
				// Read a buffer full
				GetInputStream()->Read(Buffer, 4096);

				NumRead = GetInputStream()->LastRead();

				// Write the buffer to the file
				if (NumRead > 0)
				{
					m_pOutFile->write(Buffer, NumRead);
				}
			}
		}
	}
	else
	{
		// Call the virtual function to process the output
		ProcessStdOut();
	}

	ProcessStdErr();
}