# add_library(MathFunctions mysqrt.cxx) #STEP6 (added Table.h)
add_library(MathFunctions mysqrt.cxx ${CMAKE_CURRENT_BINARY_DIR}/Table.h) #STEP6 (added Table.h)

#STEP3: Anybody linking to MathFunctions need to include the current source directory, while MathFunctions itself doesnt.
#This can become an INTERFACE usage requirement.
target_include_directories(MathFunctions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR} #STEP6
)

#STEP4: For MathFunctions, we want to install the library and header file. 
#For the application, we want to install the executable and configured header
install(TARGETS MathFunctions DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)

# #STEP5: check to see if the platform has log and exp via CheckSymbolExists module
# #On some platforms, we'll link to the m library (math library). If log and exp aren't found, require the m library and try again.
# include(CheckSymbolExists)
# #check_symbol_exists(<symbol> <files> <variable>)
# #check that the <symbol> is available after including given header <files> and store the result in a <variable>.
# #Specify the list of files in one argument as a semicolon-separated list. <variable> will be created as an internal cache.
# check_symbol_exists(log "math.h" HAVE_LOG)
# check_symbol_exists(exp "math.h" HAVE_EXP)
# if(NOT (HAVE_LOG AND HAVE_EXP))
#   unset(HAVE_LOG CACHE)
#   unset(HAVE_EXP CACHE)
#   #CMAKE_REQUIRED_LIBRARIES: list of libraries to add to the link command
#   set(CMAKE_REQUIRED_LIBRARIES "m")
#   check_symbol_exists(log "math.h" HAVE_LOG)
#   check_symbol_exists(exp "math.h" HAVE_EXP)
#   if(HAVE_EXP AND HAVE_LOG)
#     #target_link_libraries(<target> ... <item>)
#     #The command specify libraries or flags to use when linking a given target and/or its dependents
#     target_link_libraries(MathFunctions PRIVATE m)
#   endif()
# endif()

# # if log and exp are available, use the following command to specify HAVE_LOG and HAVE_EXP as PRIVATE compile definitions.
# if(HAVE_LOG AND HAVE_EXP)
#   target_compile_definitions(MathFunctions PRIVATE "HAVE_LOG" "HAVE_EXP")
# endif()

#STEP6: We would like to generate a table of precomputed values to use in the mysqrt function,
#instead of using the platform log and exp.
add_executable(MakeTable MakeTable.cxx)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
)
