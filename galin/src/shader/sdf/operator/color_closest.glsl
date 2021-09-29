vec3 sdf_color_closest(
    in float distance0, in vec3 color0,
    in float distance1, in vec3 color1)
{
    return distance0 < distance1 ? color0 : color1;
}
