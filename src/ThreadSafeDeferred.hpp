#ifndef __THREADSAFE_DEFERRED_HPP
#define __THREADSAFE_DEFERRED_HPP

#include <napi.h>
#include <functional>

#define THREADSAFE_DEFERRED_RESOLVER(result) [=](const Napi::Env env) {return result;}

typedef std::function<Napi::Value (const Napi::Env env)> createValueCb_t;

class ThreadSafeDeferred {
private:
	enum class EFate {
		UNRESOLVED,
		RESOLVED,
		REJECTED
	};
	Napi::Promise::Deferred deferred;
	EFate fate;
	createValueCb_t createValueCb;
	std::string errorMsg;
	Napi::ThreadSafeFunction tsf;
public:
	ThreadSafeDeferred(const Napi::Env env);

	inline Napi::Promise Promise() const {return this->Promise();}
	inline Napi::Env Env() const {return this->deferred.Env();}

	void Resolve(); // <- if only Resolve were virtual... But we can live without polymorphism here
	void Resolve(const createValueCb_t);
	inline void Reject() {this->Reject("");}
	void Reject(const std::string &); // <- if only Reject were virtual... But we can live without polymorphism here
};

#endif /* __THREADSAFE_DEFERRED_HPP */
