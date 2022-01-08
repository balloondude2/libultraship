#include "OTRCollisionHeaderFactory.h"

namespace OtrLib
{
    OTRCollisionHeader* OTRCollisionHeaderFactory::ReadCollisionHeader(BinaryReader* reader)
    {
        OTRCollisionHeader* colHeader = new OTRCollisionHeader();

        OTRVersion version = (OTRVersion)reader->ReadUInt32();

        switch (version)
        {
        case OTRVersion::Deckard:
        {
            OTRCollisionHeaderV0 otrCol = OTRCollisionHeaderV0();
            otrCol.ParseFileBinary(reader, colHeader);
        }
        break;
        default:
            // VERSION NOT SUPPORTED
            break;
        }

        return colHeader;
    }
};