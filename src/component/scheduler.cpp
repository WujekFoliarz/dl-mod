#include <pch.hpp>
#include "component_loader.hpp"
#include "scheduler.hpp"

#include "game/gamedll_x64_rwdi/structs.hpp"
#include "game/gamedll_x64_rwdi/symbols.hpp"
#include "function_loader.hpp"

namespace scheduler
{
	namespace 
	{
        struct task
		{
			std::function<bool()> handler{};
			std::chrono::milliseconds interval{};
			std::chrono::high_resolution_clock::time_point last_call{};
		};

        using task_list = std::vector<task>;

        class task_pipeline
        {
            public:
                void add(task&& task)
                {
                    std::lock_guard<std::mutex> new_callbacks_guard(new_callbacks_lock);
                    new_callbacks.push_back(std::move(task));
                }

                void execute()
                {
                    std::vector<task> local;

                    {
                        std::lock_guard<std::mutex> lock1(callbacks_lock);
                        std::lock_guard<std::mutex> lock2(new_callbacks_lock);

                        merge_callbacks();

                        local = std::move(callbacks);
                    }

                    for (auto i = local.begin(); i != local.end();)
                    {
                        const auto now = std::chrono::high_resolution_clock::now();
						const auto diff = now - i->last_call;

                        if (diff < i->interval)
						{
                            i++;
							continue;
						}

                        i->last_call = now;

                        const auto res = i->handler();
                        if (res == cond_end)
                        {
                            i = local.erase(i);
                        }
                        else
                        {
                            i++;
                        }
                    }

                    {
                        std::lock_guard<std::mutex> lock(callbacks_lock);
                        callbacks = std::move(local);
                    }
                }

            private:
                std::mutex callbacks_lock;
                std::mutex new_callbacks_lock;
                task_list callbacks;
                task_list new_callbacks;

                void merge_callbacks()
                {
                    for (auto& callback : new_callbacks) 
                    {
                        callbacks.push_back(callback);
                    }

                    new_callbacks.clear();
                }
        };

        volatile bool kill = false;
        std::jthread thread;
		task_pipeline pipelines[pipeline::count];

		void execute(const pipeline type)
		{
			assert(type >= 0 && type < pipeline::count);
			pipelines[type].execute();
		}

        void GameDI_stub(void* a1)
        {
            game::gamedll_x64_rwdi::GameDI::org_OnFrame(a1);
            execute(pipeline::main);
        }
	}

    void schedule(const std::function<bool()>& callback, const pipeline type, const std::chrono::milliseconds delay)
    {
        assert(type >= 0 && type < pipeline::count);

        task task;
        task.handler = callback;
        task.interval = delay;
        task.last_call = std::chrono::high_resolution_clock::now();

        pipelines[type].add(std::move(task));
    }

    void loop(const std::function<void()>& callback, const pipeline type, const std::chrono::milliseconds delay)
    {
        scheduler::schedule([callback]()
        {
            callback();
            return cond_continue;
        }, type, delay);
    }

    void once(const std::function<void()>& callback, const pipeline type, const std::chrono::milliseconds delay)
    {
        scheduler::schedule([callback]()
        {
            callback();
            return cond_end;
        }, type, delay);
    }

	class component final : public component_interface
	{
	public:
		void pre_load() override
		{

		}

		void start() override
		{
            std::cout << "[scheduler] starting" << std::endl;
            MH_CreateHook(function_loader::get_function_address("GameDI::OnFrame"), reinterpret_cast<void*>(&GameDI_stub), (void**)&game::gamedll_x64_rwdi::GameDI::org_OnFrame);
            MH_EnableHook(function_loader::get_function_address("GameDI::OnFrame"));
            std::cout << "[scheduler] started" << std::endl;
		}

        void end() override
        {
            kill = true;
        }
	};
}

REGISTER_COMPONENT(scheduler::component)