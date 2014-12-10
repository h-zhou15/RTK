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

#ifndef __rtkLUTbasedVariableI0RawToAttenuationImageFilter_h
#define __rtkLUTbasedVariableI0RawToAttenuationImageFilter_h

#include "rtkLookupTableImageFilter.h"
#include <itkNumericTraits.h>
#include <itkAddImageFilter.h>
#include <itkThresholdImageFilter.h>

namespace rtk
{
/** \class LUTbasedVariableI0RawToAttenuationImageFilter
 * \brief Performs the conversion from raw data to attenuations
 *
 * \test rtklutbasedrawtoattenuationtest.cxx
 *
 * \author S. Brousmiche
 *
 * \ingroup ImageToImageFilter
 */

template <class TInputImage, class TOutputImage>
class LUTbasedVariableI0RawToAttenuationImageFilter:
  public LookupTableImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LUTbasedVariableI0RawToAttenuationImageFilter           Self;
  typedef LookupTableImageFilter<TInputImage, TOutputImage>       Superclass;
  typedef itk::SmartPointer< Self >                               Pointer;
  typedef itk::SmartPointer< const Self >                         ConstPointer;

  typedef typename TInputImage::PixelType                     InputImagePixelType;
  typedef typename TOutputImage::PixelType                    OutputImagePixelType;
  typedef typename Superclass::FunctorType::LookupTableType   LookupTableType;
  typedef typename itk::AddImageFilter<LookupTableType>       AddLUTFilterType;
  typedef typename itk::ThresholdImageFilter<LookupTableType> ThresholdLUTFilterType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LUTbasedVariableI0RawToAttenuationImageFilter, LookupTableImageFilter);

  /** Air level I0
    */
  itkGetMacro(I0, InputImagePixelType);
  itkSetMacro(I0, InputImagePixelType);

  virtual void BeforeThreadedGenerateData();

protected:
  RTK_EXPORT LUTbasedVariableI0RawToAttenuationImageFilter();
  virtual ~LUTbasedVariableI0RawToAttenuationImageFilter() {}

private:
  LUTbasedVariableI0RawToAttenuationImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);                                //purposely not implemented

  InputImagePixelType                      m_I0;
  typename AddLUTFilterType::Pointer       m_AddLUTFilter;
  typename ThresholdLUTFilterType::Pointer m_ThresholdLUTFilter;
};
} // end namespace rtk

#ifndef ITK_MANUAL_INSTANTIATION
#include "rtkLUTbasedVariableI0RawToAttenuationImageFilter.txx"
#endif

#endif
