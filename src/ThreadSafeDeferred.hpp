#ifndef __THREADSAFE_DEFERRED_HPP
#define __THREADSAFE_DEFERRED_HPP

#include <napi.h>
#include <functional>

#define THREADSAFE_DEFERRED_RESOLVER(result) [&](const Napi::Env env) {return result;}

typedef std::function<Napi::Value (const Napi::Env env)> createValueCb_t;

class ThreadSafeDeferred : public Napi::Promise::Deferred {
private:
	enum class EFate {
		UNRESOLVED,
		RESOLVED,
		REJECTED
	};
	EFate fate;
	createValueCb_t createValueCb;
	std::string errorMsg;
	Napi::ThreadSafeFunction tsf;
public:
	ThreadSafeDeferred(const Napi::Env env);

	void Resolve(); // <- if only Resolve were virtual... But we can live without polymorphism here
	void Resolve(const createValueCb_t);
	void Reject(const std::string &); // <- if only Reject were virtual... But we can live without polymorphism here
};

#endif /* __THREADSAFE_DEFERRED_HPP */
