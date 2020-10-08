#include <dubu_event/dubu_event.h>
#include <gtest/gtest.h>

struct Foo : public dubu::event::EventEmitter {
	void DoTheThing() { Emit(1); }
};

TEST(dubu_event, emitter) {
	Foo foo;
	int counter = 0;

	{
		auto token = foo.Subscribe<int>([&](const int&) { ++counter; });
		foo.DoTheThing();
		foo.DoTheThing();
		EXPECT_EQ(counter, 2);
	}

	foo.DoTheThing();
	foo.DoTheThing();
	EXPECT_EQ(counter, 2);
}
