/*
 *  tracker/Decompressor.cpp
 *
 *  Copyright 2008 Peter Barth
 *
 *  This file is part of Milkytracker.
 *
 *  Milkytracker is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Milkytracker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Milkytracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 *  Decompressor.cpp
 *  MilkyTracker
 *
 *  Created by Peter Barth on 19.10.07.
 *
 */

#include "Decompressor.h"
#include "XMFile.h"

void DecompressorBase::removeFile(const PPSystemString& fileName)
{
	XMFile::remove(fileName);
}

bool DecompressorBase::identify()
{
	XMFile f(fileName);
	return identify(f);
}

void DecompressorBase::setFilename(const PPSystemString& filename)
{
	this->fileName = filename;
}

Decompressor::Decompressor(const PPSystemString& fileName) :
	DecompressorBase(fileName)
{
	for (pp_int32 i = 0; i < decompressorList().size(); i++)
	{
		DecompressorBase* decompressor = decompressorList().get(i)->clone();
		decompressors.add(decompressor);
	}

	adjustFilenames(fileName);
}
	
bool Decompressor::identify(XMFile& f)
{
	for (pp_int32 i = 0; i < decompressors.size(); i++)
	{
		if (decompressors.get(i)->identify(f))
		{
			return true;
		}
	}
	
	return false;
}	
	
bool Decompressor::decompress(const PPSystemString& outFileName)
{
	bool result = false;
	for (pp_int32 i = 0; i < decompressors.size(); i++)
	{
		if (decompressors.get(i)->identify())
		{
			result = decompressors.get(i)->decompress(outFileName);
			break;
		}
	}
	
	if (!result)
		removeFile(outFileName);
	
	return result;
}

DecompressorBase* Decompressor::clone()
{
	return new Decompressor(fileName);
}


void Decompressor::setFilename(const PPSystemString& filename)
{
	DecompressorBase::setFilename(fileName);
	
	adjustFilenames(fileName);
}
	
void Decompressor::adjustFilenames(const PPSystemString& filename)
{
	for (pp_int32 i = 0; i < decompressors.size(); i++)
		decompressors.get(i)->setFilename(filename);
}

PPSimpleVector<DecompressorBase>& Decompressor::decompressorList()
{
	static PPSimpleVector<DecompressorBase> decompressorList;
	return decompressorList;
}