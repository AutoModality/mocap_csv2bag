// Conventions:
//   ENU is used wrt to some fixed global frame wherein:  +X -> East; +Y -> North; +Z -> Up
//   FLU is used wrt to some relative body frame wherein: +X -> Front; +Y -> Left; +Z -> Up
//   Level refers to the fact that the frame is "Level" to an imaginary ground plane.
//       This means that by definition the Z axis is aligned with the the ENU frame.
//       Level is a way to specify the X and Y axis in a local frame (e.g. body) while fixing
//       the Z axis to align with the ENU frame.

// Frame wrt the world in ENU frame of reference
#define World_ENU "World_ENU"

// Frame wrt Asset's origin in the ENU orientation frame
#define Asset_ENU "Asset_ENU"

// Frame wrt Asset in its own origin and orientation
#define Asset_Frame "Asset_Frame"

// Frame wrt to an asset whose orientation is level to the ground
//   This means pitch and roll are irrelevant, i.e. assumed to be 0
#define Asset_Level "Asset_Level"

// Frame wrt to body of the UAV
#define body_FLU "body_FLU"

// Frame wrt to FLU body of the UAV that assumes the UAV is level to the ground.
// This means pitch and roll are irrelevant, i.e. assumed to be 0
// X -> front of UAV, Y -> left of UAV, Z -> up from UAV
#define body_Level_FLU "body_Level_FLU"
// For the time being body_FIXED and body_Level_FLU are synonymous
#define body_FIXED body_Level_FLU

// UAVs frame of reference
#define drone_ENU "drone_ENU"

// The ENU frame of reference coming from the FCU
#define FCU_ENU "FCU_ENU"

