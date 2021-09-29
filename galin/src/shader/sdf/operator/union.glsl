float sdf_union(float a, float b) {
    return min(a, b);
}

SDF_Info sdf_union(in SDF_Info si0, in SDF_Info si1) {
    SDF_Info si;
    if(si0.distance < si1.distance) {
        si.color = si0.color;
        si.distance = si0.distance;
    } else {
        si.color = si1.color;
        si.distance = si1.distance;
    }
    return si;
}
