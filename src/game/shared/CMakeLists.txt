add_sources(
	CMakeLists.txt
	bitvec.h
	mathlib.cpp
	voice_common.h
)

get_sources( GAME_SHARED_SRCS )
clear_sources()

set( GAME_SHARED_SRCS "${GAME_SHARED_SRCS}" PARENT_SCOPE )
