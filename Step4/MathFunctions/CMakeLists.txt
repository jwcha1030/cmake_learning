add_library(MathFunctions mysqrt.cxx)

#STEP3: Anybody linking to MathFunctions need to include the current source directory, while MathFunctions itself doesnt.
#This can become an INTERFACE usage requirement.
target_include_directories(MathFunctions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#STEP4: For MathFunctions, we want to install the library and header file. 
#For the application, we want to install the executable and configured header
install(TARGETS MathFunctions DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
