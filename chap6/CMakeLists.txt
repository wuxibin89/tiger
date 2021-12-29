add_library(
	frame
	x64_frame.c
	temp.c
	translate.c
)
target_link_libraries(frame parser)
target_include_directories(frame PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")