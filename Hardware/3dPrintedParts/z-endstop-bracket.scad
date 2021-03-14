l=50;
w=5;
h=10;
rod_dia=10.5;
screw_dia=3;
limit_hole_dist=9.5;
limit_dia=2.5;

fn=16;

difference() {
    union() {
        translate([-w/2, -l/2, 0]) cube([w+2,l,h]);
        translate([w/2+rod_dia/2-1, -l/2+22+rod_dia/2+4, h/2]) cylinder(h=h, d=rod_dia+8, center=true, $fn=fn);
        translate([w/2+8.5, -l/2+22+rod_dia/2+4, 0]) cube([w,l-22-rod_dia/2-4,h]);
    }
        translate([w/2+rod_dia/2-1, -l/2+22+rod_dia/2+4, h/2]) cylinder(h=h+2, d=rod_dia, center=true, $fn=fn);
        translate([w/2, -l/2+22+rod_dia/2+4, -1]) cube([8.5,l-22-rod_dia/2-4,h+2]);
    translate([0, -l/2+6, h/2]) rotate([0,90,0]) cylinder(h=w+2+3, d=limit_dia, center=true, $fn=fn);
    translate([0, -l/2+6+limit_hole_dist, h/2]) rotate([0,90,0]) cylinder(h=w+2+3, d=limit_dia, center=true, $fn=fn);
    translate([6, l/2-7, h/2]) rotate([0,90,0]) cylinder(h=w+w+10+2, d=screw_dia, center=true, $fn=fn);
    translate([-w/2, l/2-7, h/2]) rotate([0,90,0]) cylinder(h=w, d=6.3, center=true, $fn=6);
}

