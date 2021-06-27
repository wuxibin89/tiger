add_custom_command(
	OUTPUT lex.yy.c
	COMMAND flex ${CMAKE_CURRENT_SOURCE_DIR}/tiger.l
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tiger.l
	VERBATIM
)

add_library(
	lexer
	errormsg.c
	util.c
	lex.yy.c
)

add_executable(lextest main.c)

target_link_libraries(lextest lexer)

target_include_directories(lexer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_target(
	run_lextest
	COMMAND bin=lextest sh ${PROJECT_SOURCE_DIR}/test.sh
)