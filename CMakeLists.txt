
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
IF(COMMAND cmake_policy)
    CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

INCLUDE(${CMAKE_SOURCE_DIR}/IJMacros.txt)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

#Change PROJECT_NAME to the name of your project
PROJECT(itkMedialCurveTest)

IF(WIN32)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /D_SCL_SECURE_NO_WARNINGS" CACHE STRING "Flags to get rid of warnings in VS 2005 and upper" FORCE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /D_SCL_SECURE_NO_WARNINGS" CACHE STRING "Flags to get rid of warnings in VS 2005 and upper" FORCE)
ENDIF(WIN32)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)

#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
SET(Required_Packages
ITK
)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)

#Set any libraries that your project depends on.
#examples: ITKCommon, VTKRendering, etc
SET(Libraries
ITKCommon
ITKIO
)

#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project
SET(CurrentExe "ImageCompare")
ADD_EXECUTABLE(${CurrentExe} ImageCompare.cxx) 
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})
SET(CurrentExe "itkMedialCurveTest")
ADD_EXECUTABLE(${CurrentExe} itkMedialCurveTest.cpp) 
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.

ADD_TEST(ComputeCylinderMedialCurve itkMedialCurveTest ${CMAKE_SOURCE_DIR}/data/cylinder.vtk ${CMAKE_SOURCE_DIR}/data/output/cylinder_output.vtk)
ADD_TEST(TestingCylinderMedialCurve ImageCompare ${CMAKE_SOURCE_DIR}/data/output/cylinder_output.vtk ${CMAKE_SOURCE_DIR}/data/medialcurves/cylinder_medialcurve.vtk)

ADD_TEST(ComputeTorusMedialCurve itkMedialCurveTest ${CMAKE_SOURCE_DIR}/data/torus.vtk ${CMAKE_SOURCE_DIR}/data/output/torus_output.vtk)
ADD_TEST(TestingTorusMedialCurve ImageCompare ${CMAKE_SOURCE_DIR}/data/output/torus_output.vtk ${CMAKE_SOURCE_DIR}/data/medialcurves/torus_medialcurve.vtk)

ADD_TEST(ComputeAneurysm1MedialCurve itkMedialCurveTest ${CMAKE_SOURCE_DIR}/data/aneu_1.vtk ${CMAKE_SOURCE_DIR}/data/output/aneu_1_output.vtk)
ADD_TEST(TestingAneurysm1MedialCurve ImageCompare ${CMAKE_SOURCE_DIR}/data/output/aneu_1_output.vtk ${CMAKE_SOURCE_DIR}/data/medialcurves/aneu_1_medialcurve.vtk)

ADD_TEST(ComputeAneurysm2MedialCurve itkMedialCurveTest ${CMAKE_SOURCE_DIR}/data/aneu_2.vtk ${CMAKE_SOURCE_DIR}/data/output/aneu_2_output.vtk)
ADD_TEST(TestingAneurysm2MedialCurve ImageCompare ${CMAKE_SOURCE_DIR}/data/output/aneu_2_output.vtk ${CMAKE_SOURCE_DIR}/data/medialcurves/aneu_2_medialcurve.vtk)

ADD_TEST(ComputeAneurysm3MedialCurve itkMedialCurveTest ${CMAKE_SOURCE_DIR}/data/aneu_3.vtk ${CMAKE_SOURCE_DIR}/data/output/aneu_3_output.vtk)
ADD_TEST(TestingAneurysm3MedialCurve ImageCompare ${CMAKE_SOURCE_DIR}/data/output/aneu_3_output.vtk ${CMAKE_SOURCE_DIR}/data/medialcurves/aneu_3_medialcurve.vtk)

ADD_TEST(ComputeAneurysm4MedialCurve itkMedialCurveTest ${CMAKE_SOURCE_DIR}/data/aneu_4.vtk ${CMAKE_SOURCE_DIR}/data/output/aneu_4_output.vtk)
ADD_TEST(TestingAneurysm4MedialCurve ImageCompare ${CMAKE_SOURCE_DIR}/data/output/aneu_4_output.vtk ${CMAKE_SOURCE_DIR}/data/medialcurves/aneu_4_medialcurve.vtk)

#Once you're done modifying this template, you should rename it to "CMakeLists.txt"
