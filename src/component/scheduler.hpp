#pragma once

namespace scheduler
{
	enum pipeline
	{
		async = 0,
		main = 1,
		count,
	}; 

    static const bool cond_continue = false;
	static const bool cond_end = true;

    void schedule(const std::function<bool()>& callback, pipeline type = pipeline::async,
		std::chrono::milliseconds delay = std::chrono::milliseconds(0));
	void loop(const std::function<void()>& callback, pipeline type = pipeline::async,
		std::chrono::milliseconds delay = std::chrono::milliseconds(0));
	void once(const std::function<void()>& callback, pipeline type = pipeline::async,
		std::chrono::milliseconds delay = std::chrono::milliseconds(0));
}