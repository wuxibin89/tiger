add_custom_command(
	OUTPUT y.tab.h y.tab.c
	COMMAND yacc -b y -d -v ${CMAKE_CURRENT_SOURCE_DIR}/tiger.y
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tiger.y
	VERBATIM
)

add_executable(parsetest main.c y.tab.c)
target_link_libraries(parsetest lexer)

add_custom_target(
	run_parsetest
	COMMAND bin=parsetest sh ${PROJECT_SOURCE_DIR}/test.sh
)