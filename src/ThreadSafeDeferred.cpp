#include "ThreadSafeDeferred.hpp"

ThreadSafeDeferred::ThreadSafeDeferred(const Napi::Env env)
: deferred(env)
, fate(EFate::UNRESOLVED)
, createValueCb(NULL)
, errorMsg("")
, tsf {Napi::ThreadSafeFunction::New(env, Napi::Function::New(env, [](const Napi::CallbackInfo &info) {}), 
	"ThreadSafeDeferred", 0, 1, [this](Napi::Env env) {
		// this access happens from another thread. 
		// However, no synchronization is needed as
		// the other thread cannot modify this instance
		// anymore after calling Resolve or Reject.
		if (this->fate == EFate::RESOLVED) {
			if (this->createValueCb == NULL) {
				deferred.Resolve(env.Undefined());
			} else {
				deferred.Resolve(this->createValueCb(env));
			}
		} else {
			deferred.Reject(Napi::Error::New(env, this->errorMsg).Value());
		}
		try {
			delete this;
		} catch (const std::runtime_error&) {
			// Well... obviously this deferred was not allocated on heap
		}
	}
)} {}

void ThreadSafeDeferred::Resolve() {
	if (this->fate != EFate::UNRESOLVED) throw "Cannot resolve a promise which is not unresolved anymore.";
	this->fate = EFate::RESOLVED;
	this->tsf.Release();
}

void ThreadSafeDeferred::Resolve(const createValueCb_t createValueCb) {
	if (this->fate != EFate::UNRESOLVED) throw "Cannot resolve a promise which is not unresolved anymore.";
	this->createValueCb = createValueCb;
	this->fate = EFate::RESOLVED;
	this->tsf.Release();
}

void ThreadSafeDeferred::Reject(const std::string & errorMsg) {
	if (this->fate != EFate::UNRESOLVED) throw "Cannot reject a promise which is not unresolved anymore.";
	this->errorMsg = errorMsg;
	this->fate = EFate::REJECTED;
	this->tsf.Release();
}