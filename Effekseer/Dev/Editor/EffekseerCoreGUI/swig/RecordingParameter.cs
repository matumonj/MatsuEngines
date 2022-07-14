//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 4.0.2
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace Effekseer.swig {

public class RecordingParameter : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal RecordingParameter(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(RecordingParameter obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~RecordingParameter() {
    Dispose(false);
  }

  public void Dispose() {
    Dispose(true);
    global::System.GC.SuppressFinalize(this);
  }

  protected virtual void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          EffekseerNativePINVOKE.delete_RecordingParameter(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public string GetPath() {
    string ret = System.Runtime.InteropServices.Marshal.PtrToStringUni(EffekseerNativePINVOKE.RecordingParameter_GetPath(swigCPtr));
    return ret;
  }

  public string GetExt() {
    string ret = System.Runtime.InteropServices.Marshal.PtrToStringUni(EffekseerNativePINVOKE.RecordingParameter_GetExt(swigCPtr));
    return ret;
  }

  public void SetPath(string value) {
    EffekseerNativePINVOKE.RecordingParameter_SetPath(swigCPtr, value);
  }

  public void SetExt(string value) {
    EffekseerNativePINVOKE.RecordingParameter_SetExt(swigCPtr, value);
  }

  public RecordingModeType RecordingMode {
    set {
      EffekseerNativePINVOKE.RecordingParameter_RecordingMode_set(swigCPtr, (int)value);
    } 
    get {
      RecordingModeType ret = (RecordingModeType)EffekseerNativePINVOKE.RecordingParameter_RecordingMode_get(swigCPtr);
      return ret;
    } 
  }

  public int Count {
    set {
      EffekseerNativePINVOKE.RecordingParameter_Count_set(swigCPtr, value);
    } 
    get {
      int ret = EffekseerNativePINVOKE.RecordingParameter_Count_get(swigCPtr);
      return ret;
    } 
  }

  public int HorizontalCount {
    set {
      EffekseerNativePINVOKE.RecordingParameter_HorizontalCount_set(swigCPtr, value);
    } 
    get {
      int ret = EffekseerNativePINVOKE.RecordingParameter_HorizontalCount_get(swigCPtr);
      return ret;
    } 
  }

  public int OffsetFrame {
    set {
      EffekseerNativePINVOKE.RecordingParameter_OffsetFrame_set(swigCPtr, value);
    } 
    get {
      int ret = EffekseerNativePINVOKE.RecordingParameter_OffsetFrame_get(swigCPtr);
      return ret;
    } 
  }

  public int Freq {
    set {
      EffekseerNativePINVOKE.RecordingParameter_Freq_set(swigCPtr, value);
    } 
    get {
      int ret = EffekseerNativePINVOKE.RecordingParameter_Freq_get(swigCPtr);
      return ret;
    } 
  }

  public TransparenceType Transparence {
    set {
      EffekseerNativePINVOKE.RecordingParameter_Transparence_set(swigCPtr, (int)value);
    } 
    get {
      TransparenceType ret = (TransparenceType)EffekseerNativePINVOKE.RecordingParameter_Transparence_get(swigCPtr);
      return ret;
    } 
  }

  public int Scale {
    set {
      EffekseerNativePINVOKE.RecordingParameter_Scale_set(swigCPtr, value);
    } 
    get {
      int ret = EffekseerNativePINVOKE.RecordingParameter_Scale_get(swigCPtr);
      return ret;
    } 
  }

  public RecordingParameter() : this(EffekseerNativePINVOKE.new_RecordingParameter(), true) {
  }

}

}
