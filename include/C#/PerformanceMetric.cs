using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Emotiv
{
    public class PerformanceMetric
    {
        public enum IEE_PerformanceMetricAlgo_t
        {
            PM_EXCITEMENT = 0x0001,
            PM_RELAXATION = 0x0002,
            PM_STRESS     = 0x0004,
            PM_ENGAGEMENT = 0x0008,
            PM_INTEREST   = 0x0010,
            PM_FOCUS      = 0x0020
        } ;

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricEqual")]
        static extern Boolean Unmanaged_IS_PerformanceMetricEqual(IntPtr a, IntPtr b);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetExcitementLongTermScore")]
        static extern Single Unmanaged_IS_PerformanceMetricGetExcitementLongTermScore(IntPtr state);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetInstantaneousExcitementScore")]
        static extern Single Unmanaged_IS_PerformanceMetricGetInstantaneousExcitementScore(IntPtr state);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricIsActive")]
        static extern Boolean Unmanaged_IS_PerformanceMetricIsActive(IntPtr state, IEE_PerformanceMetricAlgo_t type);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetRelaxationScore")]
        static extern Single Unmanaged_IS_PerformanceMetricGetRelaxationScore(IntPtr state);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetFocusScore")]
        static extern Single Unmanaged_IS_PerformanceMetricGetFocusScore(IntPtr state);	

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetStressScore")]
        static extern Single Unmanaged_IS_PerformanceMetricGetStressScore(IntPtr state);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetEngagementBoredomScore")]
        static extern Single Unmanaged_IS_PerformanceMetricGetEngagementBoredomScore(IntPtr state);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetInterestScore")]
        static extern Single Unmanaged_IS_PerformanceMetricGetInterestScore(IntPtr state);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetInstantaneousExcitementModelParams")]
        static extern void Unmanaged_IS_PerformanceMetricGetInstantaneousExcitementModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetRelaxationModelParams")]
        static extern void Unmanaged_IS_PerformanceMetricGetRelaxationModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetEngagementBoredomModelParams")]
        static extern void Unmanaged_IS_PerformanceMetricGetEngagementBoredomModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetStressModelParams")]
        static extern void Unmanaged_IS_PerformanceMetricGetStressModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetInterestModelParams")]
        static extern void Unmanaged_IS_PerformanceMetricGetInterestModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale);

        [DllImport("edk.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "IS_PerformanceMetricGetFocusModelParams")]
        static extern void Unmanaged_IS_PerformanceMetricGetFocusModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale);


        public static Single IS_PerformanceMetricGetExcitementLongTermScore(IntPtr state)
        {
            return Unmanaged_IS_PerformanceMetricGetExcitementLongTermScore(state);
        }

        public static Single IS_PerformanceMetricGetInstantaneousExcitementScore(IntPtr state)
        {
            return Unmanaged_IS_PerformanceMetricGetInstantaneousExcitementScore(state);
        }

        public static Boolean IS_PerformanceMetricIsActive(IntPtr state, IEE_PerformanceMetricAlgo_t type)
        {
            return Unmanaged_IS_PerformanceMetricIsActive(state, type);
        }

        public static Single IS_PerformanceMetricGetFocusScore(IntPtr state)
        {
            return Unmanaged_IS_PerformanceMetricGetFocusScore(state);
        }

        public static Single IS_PerformanceMetricGetRelaxationScore(IntPtr state)
        {
            return Unmanaged_IS_PerformanceMetricGetRelaxationScore(state);
        }

        public static Single IS_PerformanceMetricGetStressScore(IntPtr state)
        {
            return Unmanaged_IS_PerformanceMetricGetStressScore(state);
        }
        public static Single IS_PerformanceMetricGetEngagementBoredomScore(IntPtr state)
        {
            return Unmanaged_IS_PerformanceMetricGetEngagementBoredomScore(state);
        }
        public static Single IS_PerformanceMetricGetInterestScore(IntPtr state)
        {
            return Unmanaged_IS_PerformanceMetricGetInterestScore(state);
        }

        public static Boolean IS_PerformanceMetricEqual(IntPtr a, IntPtr b)
        {
            return Unmanaged_IS_PerformanceMetricEqual(a, b);
        }
        public static void IS_PerformanceMetricGetInstantaneousExcitementModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale)
        {
            Unmanaged_IS_PerformanceMetricGetInstantaneousExcitementModelParams(state, out rawScore, out minScale, out maxScale);
        }
        public static void IS_PerformanceMetricGetRelaxationModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale)
        {
            Unmanaged_IS_PerformanceMetricGetRelaxationModelParams(state, out rawScore, out minScale, out maxScale);
        }
        public static void IS_PerformanceMetricGetEngagementBoredomModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale)
        {
            Unmanaged_IS_PerformanceMetricGetEngagementBoredomModelParams(state, out rawScore, out minScale, out maxScale);
        }
        public static void IS_PerformanceMetricGetStressModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale)
        {
            Unmanaged_IS_PerformanceMetricGetStressModelParams(state, out rawScore, out minScale, out maxScale);
        }
        public static void IS_PerformanceMetricGetInterestModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale)
        {
            Unmanaged_IS_PerformanceMetricGetInterestModelParams(state, out rawScore, out minScale, out maxScale);
        }

        public static void IS_PerformanceMetricGetFocusModelParams(IntPtr state, out Double rawScore, out Double minScale, out Double maxScale)
        {
            Unmanaged_IS_PerformanceMetricGetFocusModelParams(state, out rawScore, out minScale, out maxScale);
        }	
    }
}
