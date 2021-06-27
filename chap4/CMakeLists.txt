add_custom_command(
	OUTPUT y.tab.h y.tab.c
	COMMAND yacc -b y -d -v ${CMAKE_CURRENT_SOURCE_DIR}/tiger.y
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tiger.y
	VERBATIM
)

add_library(
	parser
	absyn.c
	parse.c
	prabsyn.c
	symbol.c
	table.c
	y.tab.c
)
target_link_libraries(parser lexer)
target_include_directories(parser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(asttest main.c)
target_link_libraries(asttest parser)

add_custom_target(
	run_asttest
	COMMAND bin=asttest sh ${PROJECT_SOURCE_DIR}/test.sh
)