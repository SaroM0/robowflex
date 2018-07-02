/* Author: Zachary Kingston */

#include <robowflex_library/robowflex.h>
#include <robowflex_library/detail/ur5.h>

using namespace robowflex;

int main(int argc, char **argv)
{
    startROS(argc, argv);

    auto ur5 = std::make_shared<UR5Robot>();
    ur5->initialize();

    auto scene = std::make_shared<Scene>(ur5);

    auto planner = std::make_shared<PoolPlanner>(ur5);
    planner->initialize<OMPL::UR5OMPLPipelinePlanner>();

    MotionRequestBuilder request(planner, "manipulator");
    request.setStartConfiguration({0.0677, -0.8235, 0.9860, -0.1624, 0.0678, 0.0});

    Eigen::Affine3d pose = Eigen::Affine3d::Identity();
    pose.translate(Eigen::Vector3d{-0.268, -0.826, 1.313});
    Eigen::Quaterniond orn{0, 0, 1, 0};

    auto sphere = std::make_shared<Geometry>(Geometry::ShapeType::SPHERE, Eigen::Vector3d{0.01, 0, 0});
    request.setGoalRegion("ee_link", "world",      // links
                          pose, sphere,            // position
                          orn, {0.01, 0.01, 0.01}  // orientation
    );

    // Submit a blocking call for planning. This plan is executed in a different thread
    planning_interface::MotionPlanResponse res = planner->plan(scene, request.getRequest());
    if (res.error_code_.val != moveit_msgs::MoveItErrorCodes::SUCCESS)
        return 1;

    // Submit a set of asynchronous planning calls.
    auto job1 = planner->submit(scene, request.getRequest());
    auto job2 = planner->submit(scene, request.getRequest());
    auto job3 = planner->submit(scene, request.getRequest());
    auto job4 = planner->submit(scene, request.getRequest());
    auto job5 = planner->submit(scene, request.getRequest());
    auto job6 = planner->submit(scene, request.getRequest());
    auto job7 = planner->submit(scene, request.getRequest());
    auto job8 = planner->submit(scene, request.getRequest());

    // Cancel a job (if already running, nothing happens, but if canceled before execution the job is skipped)
    job5->cancel();

    // Wait for a job to complete
    job1->wait();

    // Get results of plans. These block until results are available.
    planning_interface::MotionPlanResponse res1 = job1->get();
    planning_interface::MotionPlanResponse res2 = job2->get();
    planning_interface::MotionPlanResponse res3 = job3->get();
    planning_interface::MotionPlanResponse res4 = job4->get();

    // We canceled job5, so no result is guaranteed.
    // planning_interface::MotionPlanResponse res5 = job5->get();

    planning_interface::MotionPlanResponse res6 = job6->get();
    planning_interface::MotionPlanResponse res7 = job7->get();
    planning_interface::MotionPlanResponse res8 = job8->get();

    return 0;
}
