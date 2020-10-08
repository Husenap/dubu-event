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
		auto token = foo.Subscribe<int>([&](const int&) { ++counter; });
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
		auto token =
		    foo.Subscribe<CustomEvent>([&](const CustomEvent&) { ++counter; });
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
