#include <napi.h>

#include "BarcodeScanner.h"

Napi::Value Scan (const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();
	Napi::Value Undefined = env.Undefined();

	if (info.Length() != 5) {
		Napi::Error::New(env, "scan requires 5 arguments").ThrowAsJavaScriptException();
		return Undefined;
	}

	if (!info[0].IsBuffer()) {
		Napi::TypeError::New(env, "First argument must be a buffer").ThrowAsJavaScriptException();
		return Undefined;
	}
	Napi::TypedArrayOf<uint8_t> buffer = info[0].As<Napi::TypedArrayOf<uint8_t>>();
	uint8_t* pixels = buffer.Data();
	size_t npixels = buffer.ByteLength();
	uint32_t ncols = info[1].ToNumber().Uint32Value();
	uint32_t nrows = info[2].ToNumber().Uint32Value();
	uint32_t codeMask = info[3].ToNumber().Uint32Value();
	int scanningLevel = info[4].ToNumber().Int32Value();

	if ((size_t)(ncols * nrows) != npixels) {
		Napi::Error::New(env, "Image dimensions don't match image").ThrowAsJavaScriptException();
		return Undefined;
	}

	if (MWB_setActiveCodes(codeMask) != MWB_RT_OK) {
		Napi::TypeError::New(env, "Couldn't set barcode types").ThrowAsJavaScriptException();
		return Undefined;
	}

	MWB_setDirection(MWB_SCANDIRECTION_HORIZONTAL|MWB_SCANDIRECTION_VERTICAL);
	MWB_setScanningRect(MWB_CODE_MASK_PDF, 0, 00, 100, 100);

	if (MWB_setLevel(scanningLevel) != MWB_RT_OK) {
		Napi::TypeError::New(env, "Couldn't set scanning level").ThrowAsJavaScriptException();
		return Undefined;
	}

	uint8_t *p_data = NULL;
	int retval = MWB_scanGrayscaleImage(pixels, ncols, nrows, &p_data);
	if (retval <= 0) return env.Null();

	return Napi::Buffer<uint8_t>::New(env, p_data, retval);
}

Napi::Value Register (const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();
	Napi::Value Undefined = env.Undefined();

	if (info.Length() != 3) {
		Napi::Error::New(env, "scan requires 3 arguments").ThrowAsJavaScriptException();
		return Undefined;
	}

	uint32_t codeMask = info[0].ToNumber().Uint32Value();
	std::string userName = info[1].ToString();
	std::string key = info[2].ToString();
	int ret = MWB_registerCode(codeMask, userName.data(), key.data());
	return Napi::Number::New(env, ret);
}

Napi::Value Version (const Napi::CallbackInfo &info)
{
	unsigned int version = MWB_getLibVersion();
	return Napi::Number::New(info.Env(), version);
}


Napi::Object Init (Napi::Env env, Napi::Object exports)
{
	exports.Set(Napi::String::New(env, "scan"), Napi::Function::New(env, Scan));
	exports.Set(Napi::String::New(env, "register"), Napi::Function::New(env, Register));
	exports.Set(Napi::String::New(env, "version"), Napi::Function::New(env, Version));
	return exports;
}

NODE_API_MODULE(manatee, Init);
