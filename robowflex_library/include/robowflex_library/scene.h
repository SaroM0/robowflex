/* Author: Zachary Kingston */

#ifndef ROBOWFLEX_SCENE_
#define ROBOWFLEX_SCENE_

#include <string>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <moveit_msgs/PlanningScene.h>

#include <moveit/collision_detection/collision_matrix.h>  // for collision_detection::AllowedCollisionMatrix
#include <moveit/planning_scene/planning_scene.h>         // for planning_scene::PlanningScene
#include <moveit/robot_state/robot_state.h>               // for robot_state::RobotState

#include <robowflex_library/class_forward.h>

namespace robowflex
{
    /** \cond IGNORE */
    ROBOWFLEX_CLASS_FORWARD(Robot);
    ROBOWFLEX_CLASS_FORWARD(Geometry);
    /** \endcond */

    /** \cond IGNORE */
    ROBOWFLEX_CLASS_FORWARD(Scene);
    /** \endcond */

    /** \class robowflex::ScenePtr
        \brief A shared pointer wrapper for robowflex::Scene. */

    /** \class robowflex::SceneConstPtr
        \brief A const shared pointer wrapper for robowflex::Scene. */
    /** \brief Wrapper class around the planning scene and collision geometry.
     */
    class Scene
    {
    public:
        /** \brief Constructor.
         *  \param[in] robot Robot to construct planning scene for.
         */
        Scene(const RobotConstPtr &robot);

        /** \brief Copy Constructor.
         *  \param[in] scene Scene to copy.
         */
        Scene(const Scene &scene);

        /** \brief Assignment Copy Constructor.
         *  \param[in] scene Scene to copy.
         */
        void operator=(const Scene &scene);

        /** \name Getters and Setters
            \{ */

        /** \brief Get a const reference to the planning scene.
         *  \return The planning scene.
         */
        const planning_scene::PlanningScenePtr &getSceneConst() const;

        /** \brief Get a reference to the planning scene.
         *  \return The planning scene.
         */
        planning_scene::PlanningScenePtr &getScene();

        /** \brief Get the message that describes the current planning scene.
         *  \return The planning scene message.
         */
        moveit_msgs::PlanningScene getMessage() const;

        /** \brief Get a reference to the current robot state in the planning scene.
         *  \return The planning scene robot.
         */
        robot_state::RobotState &getCurrentState();

        /** \brief Get the current allowed collision matrix of the planning scene.
         *  \return The allowed collision matrix.
         */
        collision_detection::AllowedCollisionMatrix &getACM();

        /** \} */

        /** \name Collision Object Management
            \{ */

        /** \brief Adds or updates collision object in the planning scene.
         *  If the geometry reference is the same, the collision object is updated. Otherwise, the old object
         *  named \a name is deleted and a new one is created.
         *  \param[in] name Name of object to add or update.
         *  \param[in] geometry Geometry of object.
         *  \param[in] pose Pose of object.
         */
        void updateCollisionObject(const std::string &name, const GeometryConstPtr &geometry,
                                   const Eigen::Affine3d &pose);

        /** \brief Removes an object from the planning scene.
         *  \param[in] name Name of object to remove.
         */
        void removeCollisionObject(const std::string &name);

        /** \brief Get the current pose of a collision object.
         *  \param[in] name Name of object to get pose for.
         *  \return Pose of the object.
         */
        Eigen::Affine3d getObjectPose(const std::string &name);

        /** \brief Attach the named collision object \a name to the default end-effector of the robot.
         *  Only works if there is one end-effector in the system. Uses all end-effector links as allowed
         *  touch links.
         *  \param[in] name Name of collision to attach.
         *  \return True on success, false on failure.
         */
        bool attachObject(const std::string &name);

        /** \brief Attach the named collision object \a name to the link \a ee_link.
         *  \param[in] name Name of object to attach.
         *  \param[in] ee_link Link to attach object to.
         *  \param[in] touch_links Links the object is allowed to touch.
         *  \return True on success, false on failure.
         */
        bool attachObject(const std::string &name, const std::string &ee_link,
                          const std::vector<std::string> &touch_links);

        /** \brief Detach an object \a name from the robot.
         *  \param[in] name Name of collision to detach.
         *  \return True on success, false on failure.
         */
        bool detachObject(const std::string &name);

        /** \} */

        /** \name IO
            \{ */

        /** \brief Serialize the current planning scene to a YAML file.
         *  \param[in] file File to serialize planning scene to.
         *  \return True on success, false on failure.
         */
        bool toYAMLFile(const std::string &file);

        /** \brief Load a planning scene from a YAML file.
         *  \param[in] file File to load planning scene from.
         *  \return True on success, false on failure.
         */
        bool fromYAMLFile(const std::string &file);

        /** \} */

    private:
        planning_scene::PlanningScenePtr scene_;  ///< Underlying planning scene.
    };

    // // TODO: Document, and fix issues of the difficulties of RViz loading.
    // namespace IO
    // {
    //     class RVIZHelper
    //     {
    //     public:
    //         RVIZHelper() : nh_("~")
    //         {
    //             trajectory_pub_ = nh_.advertise<moveit_msgs::RobotTrajectory>("trajectory", 0);
    //             scene_pub_ = nh_.advertise<moveit_msgs::PlanningScene>("scene", 0);
    //             marker_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("markers", 0);
    //         }

    //         void updateTrajectory(const planning_interface::MotionPlanResponse &response)
    //         {
    //             moveit_msgs::RobotTrajectory msg;
    //             response.trajectory_->getRobotTrajectoryMsg(msg);

    //             trajectory_pub_.publish(msg);
    //         }

    //         void updateScene(const Scene &scene)
    //         {
    //             scene_pub_.publish(scene.getMessage());
    //         }

    //         void updateMarkers()
    //         {
    //             visualization_msgs::MarkerArray msg;

    //             std::vector<std::string> remove;
    //             for (auto &marker : markers_)
    //             {
    //                 msg.markers.push_back(marker.second);

    //                 if (marker.second.action == visualization_msgs::Marker::ADD)
    //                     marker.second.action = visualization_msgs::Marker::MODIFY;
    //                 else if (marker.second.action == visualization_msgs::Marker::DELETE)
    //                     remove.push_back(marker.first);
    //             }

    //             marker_pub_.publish(msg);

    //             for (auto &marker : remove)
    //                 markers_.erase(markers_.find(marker));
    //         }

    //     private:
    //         ros::NodeHandle nh_;
    //         ros::Publisher marker_pub_, trajectory_pub_, scene_pub_;

    //         std::map<std::string, visualization_msgs::Marker> markers_;
    //     };
    // }  // namespace IO
}  // namespace robowflex

#endif
