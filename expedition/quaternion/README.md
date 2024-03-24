**What is a Quaternion?**  
A Quaternion is a way to represent rotations. It is an extension of complex numbers, with the difference being the addition of two new complex numbers (j and k) and unique multiplication rules. To understand quaternions, the multiplication rules of these complex numbers must be understood. Multiplying two complex numbers involves unique multiplication rules, which are shown below in Figure 1. Interestingly, i2 = j2 = k2 = ijk = -1, but i ≠ j ≠ k. This weirdness is due to the abstractness of these complex numbers. i, being √-1, doesn’t mean anything (by itself) in real-life (you can’t have √-1 apples), but it is an abstract concept. Taking a quick glance at Figure 1., one can immediately notice that complex number multiplication isn’t commutative, instead, ij = -ji. Quaternions are in the form a + bi + cj + dk, and quaternion multiplication can be done by applying the distributive property. Assuming q1 = a + bi + cj + dk, and q2 = e + fi + gj + hk, q1 × q2 = (ae - bf - cg - dh) + (af + be + ch - dg)i + (ag - bf + ce + df)j + (ah + bg - cf + de)k (this is also shown in Figure 2). Like complex numbers, quaternion multiplication is not commutative. Quaternion multiplication (by a scalar), assuming c is a scalar and q1 is a quaternion a + bi + cj + dk, is defined by s × q1 = sa + sbi + sci + sdk. Quaternion addition, assuming q1 = a + bi + cj + dk, and q2 = e + fi + gj + hk, is defined by q1 + q2 = (a + e) + (b + f)i + (c + g)j + (d + h)k. Quaternion subtraction is based on quaternion addition, with q1 - q2 = q1 + (-1)q2. Quaternions don’t only have multiplication, addition, and subtraction, they also have conjugates, norms, and inverses. The conjugate of a quaternion, assuming q1 = a + bi + cj + dk, is denoted by ̅q1 = a - bi - cj - dk. The norm of a quaternion, a scalar, is represented by |q1|. Assuming q1 = a + bi + cj + dk, |q1| = √(a2 + b2 + c2 + d2). Finally, the inverse of a quaternion, assuming q1 = a + bi + cj + d, is represented by q1-1 =, with q1 × q1-1 = 1.
  
**How do quaternions represent rotations?**  
Understanding these earlier rules, it might seem difficult to understand how a quaternion can represent a rotation, being 4-dimensional (having a, b, c, and d), but quaternions are used to prevent a major flaw with Euler angles. Euler angles are a system that stores angles as Pitch, Roll, and Yaw. A major flaw with this system is that is susceptible to a problem called gimbal lock. (Figure 3). When the axes of a gimbal become parallel, the gimbal gets stuck, only being able to represent rotations in two axes from then on. Quaternions, by adding a 4th dimension, are no longer susceptible to this. Assuming a normalized axis of rotation x, y, z, and an angle θ, a quaternion can represent a rotation q = cos(θ/2) + sin(θ/2)xi + sin(θ/2)yj +  + sin(θ/2)zk. To rotate a point x, y, z around the quaternion q, 0 + xnewi + ynewj + znewk = q × (0 + xi + yj + zk) × q-1.

(Fig. 1)

(Fig. 2)

(Fig. 3)

**Project Descriptions**  
* obj_file - This is a library to parse .obj files. It also parses .mtl files, so that .obj files appear in the correct color.  
  *It stores the .obj file’s vertices and normals in OpenGL buffers for easy future use.  
*quaternion_demo - This is a library with a Quaternion class that implements various quaternion operations with some test programs to verify the class works correctly.
*viewer_with_euler - This is a .obj viewer using Euler angles as a baseline. It has 3 spinboxes, each storing the x, y, and z (roll, pitch, and yaw). It is not possible to use the mouse to drag the camera around. Diffuse lighting is implemented.
  *It uses obj_file to parse the provided .obj file.
https://youtu.be/APsEHQz6kWw
*viewer_with_quaternion - This is a .obj viewer using Quaternions, built off viewer_with_euler. It is possible to use the mouse to drag the camera around. Diffuse lighting is implemented.
  *It uses obj_file to parse the provided .obj file.
*  Eigen’s Quaternion class is used instead of quaternion_demo’s quaternion library as it is more advanced and has more features.
https://www.youtube.com/watch?v=RonZaKVwmh0
