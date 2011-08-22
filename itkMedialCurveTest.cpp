/*=========================================================================

  Copyright : (C)opyright (Pompeu Fabra University) 2007++
              See COPYRIGHT statement in top level directory.
  Authors   : Ignacio Larrabide && Xavier Mellado
  Modified  : $Author: $
  Purpose   : Application for computing of medial curve inside a surface
              from the distance map representation of a surface.
  Date      : $Date: $
  Version   : $Revision: $

=========================================================================*/

//ITK
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkRecursiveGaussianImageFilter.h>
#include <itkGradientImageFilter.h>
#include <itkAverageOutwardFluxImageFilter.h>
#include <itkMedialCurveImageFilter.h>

using namespace std;

//Usage example: 

void usage( char *app_name )
{
	cerr << "Usage: " << app_name << " [input image] [output image]  <parameters>        " << endl;
	cerr << "    -sigma <float> <- sigma for recursive gaussian filter [1.0]             " << endl;
	cerr << "    -threshold <float> <- threshold for the Average Outward Flux [0.0]      " << endl;
	exit(1);
}

int main( int argc, char **argv )
{    
	char *input_name = NULL, *output_name = NULL;
	double sigma = 0.5, threshold = 0.0;

	if ( argc < 3 )
		usage( argv[0] );

	input_name  = argv[1];
	argc--;
	argv++;
	output_name = argv[1];
	argc--;
	argv++;

	int ok;
	
	// Parse input arguments
	while ( argc > 1 )
	{
		ok = false;

		if ( ( ok == false ) && ( strcmp( argv[1], "-sigma" ) == 0 ) )
		{
			argc--;
			argv++;
			sigma = atof( argv[1] );
			argc--;
			argv++;
			ok = true;
		}

		if ( ( ok == false ) && ( strcmp( argv[1], "-threshold" ) == 0 ) )
		{
			argc--;
			argv++;
			threshold = atof( argv[1] );
			argc--;
			argv++;
			ok = true;
		}

		if ( ok == false )
		{
			cerr << "Can not parse argument " << argv[1] << endl;
			usage( argv[0] );
		}
	}

	typedef float PixelType;
	typedef itk::Image< PixelType, 3 > ImageType;
	typedef itk::ImageFileReader< ImageType > ImageReaderType;

	// Read the input file
	ImageReaderType::Pointer reader = ImageReaderType::New();
	reader->SetFileName( input_name );
	try
	{
		reader->Update();
	}
	catch ( itk::ExceptionObject &err)
	{
		std::cout << "ExceptionObject caught !" << std::endl; 
		std::cout << err << std::endl; 
		return -1;
	}

	ImageType::Pointer distance = reader->GetOutput();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1.	Compute the associated average outward flux
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// To have a good quality gradient of the distance map, perform a light smooth over it. Define  
	// convolution kernels in each direction and use them recursively. 
	typedef itk::RecursiveGaussianImageFilter< ImageType, ImageType > RecursiveGaussianFilterType;
	RecursiveGaussianFilterType::Pointer gaussianFilterX = RecursiveGaussianFilterType::New();
	RecursiveGaussianFilterType::Pointer gaussianFilterY = RecursiveGaussianFilterType::New();
	RecursiveGaussianFilterType::Pointer gaussianFilterZ = RecursiveGaussianFilterType::New();

	gaussianFilterX->SetDirection( 0 );
	gaussianFilterY->SetDirection( 1 );
	gaussianFilterZ->SetDirection( 2 );

	gaussianFilterX->SetOrder( RecursiveGaussianFilterType::ZeroOrder );
	gaussianFilterY->SetOrder( RecursiveGaussianFilterType::ZeroOrder );
	gaussianFilterZ->SetOrder( RecursiveGaussianFilterType::ZeroOrder );

	gaussianFilterX->SetNormalizeAcrossScale( false );
	gaussianFilterY->SetNormalizeAcrossScale( false );
	gaussianFilterZ->SetNormalizeAcrossScale( false );

	gaussianFilterX->SetInput( distance );
	gaussianFilterY->SetInput( gaussianFilterX->GetOutput() );
	gaussianFilterZ->SetInput( gaussianFilterY->GetOutput() );

	gaussianFilterX->SetSigma( sigma );
	gaussianFilterY->SetSigma( sigma );
	gaussianFilterZ->SetSigma( sigma );

	typedef itk::GradientImageFilter< ImageType, PixelType, PixelType > GradientFilterType;

	// Compute the gradient.
	GradientFilterType::Pointer gradientFilter = GradientFilterType::New();
	gradientFilter->SetInput( gaussianFilterZ->GetOutput() );
	gradientFilter->SetInput( gaussianFilterY->GetOutput() );

	// Compute the average outward flux.
	typedef itk::AverageOutwardFluxImageFilter< ImageType, PixelType, GradientFilterType::OutputImageType::PixelType > AOFFilterType;
	AOFFilterType::Pointer aofFilter = AOFFilterType::New();
	aofFilter->SetInput( distance );
	aofFilter->SetGradientImage( gradientFilter->GetOutput() );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. Compute the skeleton
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef itk::MedialCurveImageFilter< ImageType > MedialCurveFilter;
	MedialCurveFilter::Pointer medialFilter = MedialCurveFilter::New();
	medialFilter->SetInput( distance );
	medialFilter->SetAverageOutwardFluxImage( aofFilter->GetOutput() );
	medialFilter->SetThreshold( threshold );

	// Write the output image containing the skeleton.
	typedef itk::ImageFileWriter< MedialCurveFilter::TOutputImage > WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetInput( medialFilter->GetOutput() );
	writer->SetFileName( output_name );
	try
	{
		writer->Update();
	}
	catch ( itk::ExceptionObject &err )
	{
		std::cout << "ExceptionObject caught !" << std::endl; 
		std::cout << err << std::endl; 
		return -1;
	}

	return 0;	
}
