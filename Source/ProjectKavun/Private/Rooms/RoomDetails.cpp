#include "Rooms/RoomDetails.h"

FRoomPieceInfo::FRoomPieceInfo()
	: RelativeLocation{0, 0},
	  NeighborOccupiedFlags{
			  {EDirections_Up, EOccupiedFlag::Anything},
			  {EDirections_Down, EOccupiedFlag::Anything},
			  {EDirections_Left, EOccupiedFlag::Anything},
			  {EDirections_Right, EOccupiedFlag::Anything}
	  }
{
}

FRoomInfo::FRoomInfo()
	: Shape{ERoomShape::Square},
	  Type{ERoomType::Default},
	  PiecesInfo{FRoomPieceInfo{}}
{
}
