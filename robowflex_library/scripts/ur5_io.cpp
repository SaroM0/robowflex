#include <robowflex_library/robowflex.h>
#include <robowflex_library/detail/ur5.h>

using namespace robowflex;

int main(int argc, char **argv)
{
    startROS(argc, argv);

    auto ur5 = std::make_shared<UR5Robot>();
    ur5->initialize();

    auto scene = std::make_shared<Scene>(ur5);
    scene->fromYAMLFile("package://robowflex_library/yaml/test.yml");

    {
        IO::Bag bag_out("scene.bag", IO::Bag::WRITE);
        bag_out.addMessage("scene", scene->getMessage());
    }

    {
        IO::Bag bag_in("scene.bag", IO::Bag::READ);
        auto msgs = bag_in.getMessages<moveit_msgs::PlanningScene>({"scene"});
    }

    return 0;
}