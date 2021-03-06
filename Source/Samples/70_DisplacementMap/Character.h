//
// Copyright (c) 2008-2016 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include <Urho3D/Input/Controls.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Physics/RigidBody.h>

using namespace Urho3D;

//=============================================================================
//=============================================================================
const int CTRL_FORWARD = 1;
const int CTRL_BACK = 2;
const int CTRL_LEFT = 4;
const int CTRL_RIGHT = 8;
const int CTRL_JUMP = 16;

const float MOVE_FORCE = 1.2f;
const float INAIR_MOVE_FORCE = 0.02f;
const float BRAKE_FORCE = 0.2f;
const float JUMP_FORCE = 7.0f;
const float YAW_SENSITIVITY = 0.1f;
const float INAIR_THRESHOLD_TIME = 0.1f;

//=============================================================================
//=============================================================================
#define FORCE_MULTIPLYER_ON_PLATFORM        2.5f


URHO3D_EVENT(E_FOOTSTEP, FootstepEvent)
{
	URHO3D_PARAM(P_POS, Pos);
	URHO3D_PARAM(P_DIR, Dir);
	URHO3D_PARAM(P_FOOT, Foot); // 0 = right, 1 = left
}

//=============================================================================
//=============================================================================
/// Character component, responsible for physical movement according to controls, as well as animation.
class Character : public LogicComponent
{
    URHO3D_OBJECT(Character, LogicComponent);

public:
    /// Construct.
    Character(Context* context);
    
    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    
    /// Handle startup. Called by LogicComponent base class.
    virtual void Start();
    /// Handle physics world update. Called by LogicComponent base class.
    virtual void FixedUpdate(float timeStep);
    
    void SetOnMovingPlatform(RigidBody *platformBody)
    { 
        onMovingPlatform_ = (platformBody != NULL);
        platformBody_ = platformBody; 
    }
    const bool IsOnGround() const { return onGround_; }
    const bool IsJumping() const { return isJumping_; }

public:
    /// Movement controls. Assigned by the main program each frame.
    Controls controls_;

protected:
    /// Handle physics collision event.
    void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
    void HandleAnimationTrigger(StringHash eventType, VariantMap& eventData);
    void SendJumpLandEvent();
    void SendFootstepEvent(const Vector3 &pos, const Vector3 &dir, int whichFoot);
    
protected:
    /// Grounded flag for movement.
    bool onGround_;

    /// Jump flag.
    bool okToJump_;
    /// In air timer. Due to possible physics inaccuracy, character can be off ground for max. 1/10 second and still be allowed to move.
    float inAirTimer_;
    bool isPlayingJumpLoopAnim_;

    // extra vars
    Vector3 curMoveDir_;
    bool isJumping_;
    bool jumpStarted_;

    // platform
    bool onMovingPlatform_;
    WeakPtr<RigidBody> platformBody_;

    // water contact
    bool inWater_;
    Vector3 waterContatct_;

};
