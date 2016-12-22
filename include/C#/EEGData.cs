using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Emotiv
{
    public class EEGData
    {
        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataCreate")]
        static extern IntPtr Unmanaged_IEE_DataCreate();

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataFree")]
        static extern void Unmanaged_IEE_DataFree(IntPtr hData);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataUpdateHandle")]
        static extern Int32 Unmanaged_IEE_DataUpdateHandle(UInt32 userId, IntPtr hData);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataGet")]
        static extern Int32 Unmanaged_IEE_DataGet(IntPtr hData, EdkDll.IEE_DataChannel_t channel, Double[] buffer, UInt32 bufferSizeInSample);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataGetMultiChannels")]
        static extern Int32 Unmanaged_IEE_DataGetMultiChannels(IntPtr hData, EdkDll.IEE_DataChannel_t[] channelList, UInt32 nChannel, Double[][] buffer, UInt32 bufferSizeInSample);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataGetNumberOfSample")]
        static extern Int32 Unmanaged_IEE_DataGetNumberOfSample(IntPtr hData, out UInt32 nSampleOut);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataSetBufferSizeInSec")]
        static extern Int32 Unmanaged_IEE_DataSetBufferSizeInSec(Single bufferSizeInSec);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataGetBufferSizeInSec")]
        static extern Int32 Unmanaged_IEE_DataGetBufferSizeInSec(out Single pBufferSizeInSecOut);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataAcquisitionEnable")]
        static extern Int32 Unmanaged_IEE_DataAcquisitionEnable(UInt32 userId, Boolean enable);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataAcquisitionIsEnabled")]
        static extern Int32 Unmanaged_IEE_DataAcquisitionIsEnabled(UInt32 userId, out Boolean pEnableOut);
        
        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataSetMarker")]
        static extern Int32 Unmanaged_IEE_DataSetMarker(UInt32 userId, Int32 marker);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IEE_DataGetSamplingRate")]
        static extern Int32 Unmanaged_IEE_DataGetSamplingRate(UInt32 userId, out UInt32 pSamplingRate);


        public static IntPtr IEE_DataCreate()
        {
            return Unmanaged_IEE_DataCreate();
        }

        public static void IEE_DataFree(IntPtr hData)
        {
            Unmanaged_IEE_DataFree(hData);
        }

        public static Int32 IEE_DataUpdateHandle(UInt32 userId, IntPtr hData)
        {
            return Unmanaged_IEE_DataUpdateHandle(userId, hData);
        }

        public static Int32 IEE_DataGet(IntPtr hData, EdkDll.IEE_DataChannel_t channel, Double[] buffer, UInt32 bufferSizeInSample)
        {
            return Unmanaged_IEE_DataGet(hData, channel, buffer, bufferSizeInSample);
        }

        public static Int32 IEE_DataGetMultiChannel(IntPtr hData, EdkDll.IEE_DataChannel_t[] channelList, UInt32 nChannel, Double[][] buffer, UInt32 bufferSizeInSample)
        {
            return Unmanaged_IEE_DataGetMultiChannels(hData, channelList, nChannel, buffer, bufferSizeInSample);
        }

        public static Int32 IEE_DataGetNumberOfSample(IntPtr hData, out UInt32 nSampleOut)
        {
            return Unmanaged_IEE_DataGetNumberOfSample(hData, out nSampleOut);
        }

        public static Int32 IEE_DataSetBufferSizeInSec(Single bufferSizeInSec)
        {
            return Unmanaged_IEE_DataSetBufferSizeInSec(bufferSizeInSec);
        }

        public static Int32 IEE_DataGetBufferSizeInSec(out Single pBufferSizeInSecOut)
        {
            return Unmanaged_IEE_DataGetBufferSizeInSec(out pBufferSizeInSecOut);
        }

        public static Int32 IEE_DataAcquisitionEnable(UInt32 userId, Boolean enable)
        {
            return Unmanaged_IEE_DataAcquisitionEnable(userId, enable);
        }

        public static Int32 IEE_DataAcquisitionIsEnabled(UInt32 userId, out Boolean pEnableOut)
        {
            return Unmanaged_IEE_DataAcquisitionIsEnabled(userId, out pEnableOut);
        }

        public static Int32 IEE_DataSetMarker(UInt32 userId, Int32 marker)
        {
            return Unmanaged_IEE_DataSetMarker(userId, marker);
        }

        public static Int32 IEE_DataGetSamplingRate(UInt32 userId, out UInt32 pSamplingRateOut)
        {
            return Unmanaged_IEE_DataGetSamplingRate(userId, out pSamplingRateOut);
        }
    }
}
