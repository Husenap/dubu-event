#include <dubu_event/dubu_event.h>
#include <gtest/gtest.h>

struct CustomEvent {
	int data;
};

struct Foo : public dubu::event::EventEmitter {
	template <typename T>
	void DoTheThing() {
		Emit(T());
	}
};

TEST(dubu_event, emitter) {
	Foo foo;
	int counter = 0;

	{
		auto token = foo.RegisterListener<int>([&](const int&) { ++counter; });
		foo.DoTheThing<int>();
		foo.DoTheThing<int>();
		EXPECT_EQ(counter, 2);
	}

	foo.DoTheThing<int>();
	foo.DoTheThing<CustomEvent>();
	EXPECT_EQ(counter, 2);
}

TEST(dubu_event, custom_event) {
	Foo foo;
	int counter = 0;

	{
		auto token = foo.RegisterListener<CustomEvent>(
		    [&](const CustomEvent&) { ++counter; });
		foo.DoTheThing<CustomEvent>();
		foo.DoTheThing<CustomEvent>();
		foo.DoTheThing<int>();
		foo.DoTheThing<int>();
		EXPECT_EQ(counter, 2);
	}

	foo.DoTheThing<CustomEvent>();
	foo.DoTheThing<int>();
	EXPECT_EQ(counter, 2);
}

struct Bar : dubu::event::EventSubscriber {
	Bar(Foo& foo, Foo& foo1) {
		Subscribe<CustomEvent>([](const auto&) {}, foo);
		Subscribe<int>([&](const int&) { ++counter; }, foo);
		Subscribe<int>([&](const int&) { --counter; }, foo1);
	}
	int counter = 0;
};

TEST(dubu_event, event_subscriber) {
	Foo foo;
	Foo foo1;
	Bar bar(foo, foo1);

	EXPECT_EQ(bar.counter, 0);

	foo.DoTheThing<int>();
	foo.DoTheThing<CustomEvent>();
	foo.DoTheThing<int>();
	foo1.DoTheThing<int>();

	EXPECT_EQ(bar.counter, 1);

	foo.DoTheThing<int>();
	foo.DoTheThing<CustomEvent>();
	foo.DoTheThing<int>();

	EXPECT_EQ(bar.counter, 3);

	foo1.DoTheThing<int>();

	EXPECT_EQ(bar.counter, 2);
}

struct Window : dubu::event::EventEmitter {
	void Update() { Emit<CustomEvent>(); }
};
struct Input : dubu::event::EventEmitter, dubu::event::EventSubscriber {
	Input(Window& window) {
		Subscribe<CustomEvent>([this](const auto&) { Emit<CustomEvent>(); },
		                       window);
	}
};

TEST(dubu_event, event_emitter_subscriber) {
	Window window;
	Input  input(window);

	int counter = 0;

	{
		auto token = input.RegisterListener<CustomEvent>(
		    [&](const auto&) { ++counter; });

		window.Update();
		window.Update();
	}

	window.Update();
	window.Update();

	EXPECT_EQ(counter, 2);
}
