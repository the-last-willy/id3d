
// Props.

FieldValue base(in vec3 p) {
    p = ((vec3(1./15., 1., 1./15.) * p - vec3(0., -4., 0.))).xzy;
    return FieldValue(
        vec3(0.),
        cylinder(p, vec3(0.), 1.f, 1.f));
}

FieldValue pillar(in vec3 p) {
    return FieldValue(
        vec3(252, 212, 64) / 255,
        point(p, vec3(0., 0., 0.), 1., 1.));
}

// Planets.

FieldValue sun_disc(in vec3 p) {
    FieldValue fv = FieldValue(
        vec3(252, 212, 64) / 255,
        point(p, vec3(0., 0., 0.), 1., 1.));
    fv = Union(fv, FieldValue(
        vec3(252, 212, 64) / 255,
        segment(p, vec3(0., -5., 0.), vec3(0., 0., 0.), 1., 0.2)));
    return fv;
}

FieldValue mercury_disc(in vec3 p) {
    p = rotateY(p, -1.7 * iTime);
    p -= vec3(1., 0., 0.);

    FieldValue fv = FieldValue(
        vec3(252, 212, 64) / 255,
        point(p, vec3(0.), 1., 1.));
    fv = Union(fv, FieldValue(
        vec3(252, 212, 64) / 255,
        segment(p, vec3(0., -5., 0.), vec3(0., 0., 0.), 1., 0.2)));
    return fv;
}

FieldValue venus_disc(in vec3 p) {
    p = rotateY(p, -1.6 * iTime);
    p -= vec3(2., 0., 0.);

    FieldValue fv = FieldValue(
        vec3(252, 212, 64) / 255,
        point(p, vec3(0.), 1., 1.));
    fv = Union(fv, FieldValue(
        vec3(252, 212, 64) / 255,
        segment(p, vec3(0., -5., 0.), vec3(0., 0., 0.), 1., 0.2)));
    return fv;
}

FieldValue earth_disc(in vec3 p) {
    p = rotateY(p, -1.5 * iTime);
    p -= vec3(3., 0., 0.);

    FieldValue fv = FieldValue(
        vec3(252, 212, 64) / 255,
        point(p, vec3(0.), 1., 1.));
    fv = Union(fv, FieldValue(
        vec3(252, 212, 64) / 255,
        segment(p, vec3(0., -5., 0.), vec3(0., 0., 0.), 1., 0.2)));
    return fv;
}

FieldValue mars_disc(in vec3 p) {
    p = rotateY(p, -1.4 * iTime);
    p -= vec3(4., 0., 0.);

    FieldValue fv = FieldValue(
        vec3(252, 212, 64) / 255,
        point(p, vec3(0.), 1., 1.));
    fv = Union(fv, FieldValue(
        vec3(252, 212, 64) / 255,
        segment(p, vec3(0., -5., 0.), vec3(0., 0., 0.), 1., 0.2)));
    return fv;
}

FieldValue jupiter_disc(in vec3 p) {
    p = rotateY(p, -1.3 * iTime);
    p -= vec3(5., 0., 0.);

    FieldValue fv = FieldValue(
        vec3(252, 212, 64) / 255,
        point(p, vec3(0.), 1., 1.));
    fv = Union(fv, FieldValue(
        vec3(252, 212, 64) / 255,
        segment(p, vec3(0., -5., 0.), vec3(0., 0., 0.), 1., 0.2)));
    return fv;
}

FieldValue saturn_disc(in vec3 p) {
    p = rotateY(p, -1.2 * iTime);
    p -= vec3(6., 0., 0.);

    FieldValue fv = FieldValue(
        vec3(252, 212, 64) / 255,
        point(p, vec3(0.), 1., 1.));
    fv = Union(fv, FieldValue(
        vec3(252, 212, 64) / 255,
        segment(p, vec3(0., -5., 0.), vec3(0., 0., 0.), 1., 0.2)));
    return fv;
}

FieldValue neptune_disc(in vec3 p) {
    p = rotateY(p, -1.1 * iTime);
    p -= vec3(7., 0., 0.);

    FieldValue fv = FieldValue(
        vec3(252, 212, 64) / 255,
        point(p, vec3(0.), 1., 1.));
    fv = Union(fv, FieldValue(
        vec3(252, 212, 64) / 255,
        segment(p, vec3(0., -5., 0.), vec3(0., 0., 0.), 1., 0.2)));
    return fv;
}

FieldValue uranus_disc(in vec3 p) {
    p = rotateY(p, -1 * iTime);
    p -= vec3(8., 0., 0.);

    FieldValue fv = FieldValue(
        vec3(252, 212, 64) / 255,
        point(p, vec3(0.), 1., 1.));
    fv = Union(fv, FieldValue(
        vec3(252, 212, 64) / 255,
        segment(p, vec3(0., -5., 0.), vec3(0., 0., 0.), 1., 0.2)));
    return fv;
}

FieldValue orrery(in vec3 p) {
    FieldValue fv = FieldValue(vec3(1.), 0.);
    fv = Union(fv, sun_disc(p));
    fv = Union(fv, mercury_disc(p));
    fv = Union(fv, venus_disc(p));
    fv = Union(fv, earth_disc(p));
    fv = Union(fv, mars_disc(p));
    fv = Union(fv, jupiter_disc(p));
    fv = Union(fv, saturn_disc(p));
    fv = Union(fv, neptune_disc(p));
    fv = Union(fv, uranus_disc(p));

    fv = Union(fv, base(p));

    return fv;
}
