﻿# WARNING: Please don't edit this file. It was generated by Python/WinRT

from pyrt import _import_ns
import typing
import enum

__ns__ = _import_ns("Windows.AI.MachineLearning")

try:
    import pyrt.windows.foundation
except:
    pass

try:
    import pyrt.windows.foundation.collections
except:
    pass

try:
    import pyrt.windows.graphics
except:
    pass

try:
    import pyrt.windows.graphics.directx.direct3d11
except:
    pass

try:
    import pyrt.windows.graphics.imaging
except:
    pass

try:
    import pyrt.windows.media
except:
    pass

try:
    import pyrt.windows.storage
except:
    pass

try:
    import pyrt.windows.storage.streams
except:
    pass

class LearningModelDeviceKind(enum.IntEnum):
    Default = 0
    Cpu = 1
    DirectX = 2
    DirectXHighPerformance = 3
    DirectXMinPower = 4

class LearningModelFeatureKind(enum.IntEnum):
    Tensor = 0
    Sequence = 1
    Map = 2
    Image = 3

class TensorKind(enum.IntEnum):
    Undefined = 0
    Float = 1
    UInt8 = 2
    Int8 = 3
    UInt16 = 4
    Int16 = 5
    Int32 = 6
    Int64 = 7
    String = 8
    Boolean = 9
    Float16 = 10
    Double = 11
    UInt32 = 12
    UInt64 = 13
    Complex64 = 14
    Complex128 = 15

ImageFeatureDescriptor = __ns__.ImageFeatureDescriptor
ImageFeatureValue = __ns__.ImageFeatureValue
LearningModel = __ns__.LearningModel
LearningModelBinding = __ns__.LearningModelBinding
LearningModelDevice = __ns__.LearningModelDevice
LearningModelEvaluationResult = __ns__.LearningModelEvaluationResult
LearningModelSession = __ns__.LearningModelSession
MapFeatureDescriptor = __ns__.MapFeatureDescriptor
SequenceFeatureDescriptor = __ns__.SequenceFeatureDescriptor
TensorBoolean = __ns__.TensorBoolean
TensorDouble = __ns__.TensorDouble
TensorFeatureDescriptor = __ns__.TensorFeatureDescriptor
TensorFloat = __ns__.TensorFloat
TensorFloat16Bit = __ns__.TensorFloat16Bit
TensorInt16Bit = __ns__.TensorInt16Bit
TensorInt32Bit = __ns__.TensorInt32Bit
TensorInt64Bit = __ns__.TensorInt64Bit
TensorInt8Bit = __ns__.TensorInt8Bit
TensorString = __ns__.TensorString
TensorUInt16Bit = __ns__.TensorUInt16Bit
TensorUInt32Bit = __ns__.TensorUInt32Bit
TensorUInt64Bit = __ns__.TensorUInt64Bit
TensorUInt8Bit = __ns__.TensorUInt8Bit
ILearningModelFeatureDescriptor = __ns__.ILearningModelFeatureDescriptor
ILearningModelFeatureValue = __ns__.ILearningModelFeatureValue
ILearningModelOperatorProvider = __ns__.ILearningModelOperatorProvider
ITensor = __ns__.ITensor
