/*=========================================================================
 *
 *  Copyright RTK Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef __rtkLagCorrectionImageFilter_h
#define __rtkLagCorrectionImageFilter_h

#include <itkInPlaceImageFilter.h>
#include <itkVector.h>
#include <itkArray.h>
#include <itkSimpleFastMutexLock.h>
#include <itkRealTimeClock.h>

#include <vector>

using namespace std;

namespace rtk
{

/** \class LagCorrectionImageFilter
 * \brief Classical Linear Time Invariant Lag correction
 *
 * Recursive correction algorithm for detector decay characteristics. 
 * Based on [Hsieh, Proceedings of SPIE, 2000]
 *
 * The IRF (Impulse Response Function) is given by:
 *    \f$h(k)=b_0 \delta(k) + \sum_{n=1}^N b_n e^{-a_n k}\f$
 * where \f$k\f$ is the discrete time, \f$N\f$ is the model order (number of exponentials), 
 * \f$\delta(k)\f$ is the impulse function and the \f${a_n, b_n}_{n=1:N}\f$ parameters are respectively the exponential rates and
 * lag coefficients to be provided. The sum of all $b_n$ must be normalized such that h(0) equals 1.
 * 
 * The parameters are typically estimated from either RSRF (Rising Step RF) or FSRF (Falling Step RF) response functions.
 *
 * \test rtklagcorrectiontest.cxx
 *
 * \author Sebastien Brousmiche
 *
 * \ingroup ImageToImageFilter
*/

template< typename TImage, unsigned ModelOrder >
class ITK_EXPORT LagCorrectionImageFilter : public itk::InPlaceImageFilter< TImage, TImage>
{
public:
   
  /** Standard class typedefs. */
  typedef LagCorrectionImageFilter                   Self;
	typedef itk::InPlaceImageFilter< TImage, TImage >  Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
	typedef itk::SmartPointer< const Self >            ConstPointer;
	
  /** Method for creation through the object factory. */
	itkNewMacro(Self)

 /** Run-time type information (and related methods). */
	itkTypeMacro(LagCorrectionImageFilter, ImageToImageFilter)
		
	typedef typename TImage::RegionType                ImageRegionType;
	typedef typename TImage::SizeType                  ImageSizeType;
	typedef typename TImage::PixelType                 PixelType;
	typedef itk::Vector<float, ModelOrder>             VectorType;
	typedef itk::RealTimeClock                         itkClockType;
	typedef itk::RealTimeStamp::TimeRepresentationType itkTimeType;
	typedef itk::Image<VectorType, 3>                  StateType;
	typedef typename StateType::Pointer                StateTypePtr;

	/** Get and Set the model parameters A*/
	itkGetMacro(A, VectorType)
	itkSetMacro(A, VectorType)

	/** Get and Set the model parameters A*/
	itkGetMacro(B, VectorType)
	itkSetMacro(B, VectorType)

	void Initialize();

	void ResetInternalState();

protected:
  LagCorrectionImageFilter();
  virtual ~LagCorrectionImageFilter(){}
  
	virtual void BeforeThreadedGenerateData();

	virtual void ThreadedGenerateData(const ImageRegionType & outputRegionForThread, ThreadIdType threadId);
	
	virtual void AfterThreadedGenerateData();
	
	VectorType m_A;           // a_n coefficients
	VectorType m_B;           // b coefficients
	VectorType m_expma;       // exp(-a)
	VectorType m_bexpma;      // b exp(-a)

private:
  LagCorrectionImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented
	
	ImageSizeType m_Size;
	unsigned int m_M;         // Full size
	StateTypePtr m_S;         // State variable

	itk::SimpleFastMutexLock m_mutex;

	unsigned int m_ImageId;     // Image number
	unsigned int m_nThreads;

	float m_thAvgCorr;
	std::vector<float> m_avgCorrection; 
	
	itkClockType::Pointer m_clock;
};

} //namespace RTK

#ifndef ITK_MANUAL_INSTANTIATION
#include "rtkLagCorrectionImageFilter.txx"
#endif

#endif

