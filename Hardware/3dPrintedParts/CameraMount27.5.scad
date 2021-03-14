l=50;
w=4;
h=10;
rod_dia=8.5;
screw_dia=3;
limit_hole_dist=9.5;
limit_dia=2.5;

b_rim=w;
b_thick=w;
bz_l=27.5+b_rim+b_rim;
by=27.5+b_thick;
bx=rod_dia+b_rim+b_rim;;
bmh_d=5;
bmh_w=8;


fn=16;

difference() {
    union() {
        translate([-w/2, -l/2+22+rod_dia/2+4, 0]) cube([w+2,l-22-rod_dia/2-4,h]);
        translate([w/2+rod_dia/2-1, -l/2+22+rod_dia/2+4, h/2]) cylinder(h=h, d=rod_dia+8, center=true, $fn=fn);
        translate([w/2+rod_dia-2, -l/2+22+rod_dia/2+4, 0]) cube([w,l-22-rod_dia/2-4,h]);
        
        translate([w/2+rod_dia/2-1-(bx/2), -l/2-rod_dia/2, 0]) difference() {
        cube([bx,by+3,b_thick]);
        translate([bx/2-(bmh_w/2), (by-b_thick)/2, b_thick/2]) cylinder(h=b_thick+2,d=bmh_d, center=true, $fn=fn);
        translate([bx/2+(bmh_w/2), (by-b_thick)/2, b_thick/2]) cylinder(h=b_thick+2,d=bmh_d, center=true, $fn=fn);
    }
    }
        translate([w/2+rod_dia/2-1, -l/2+22+rod_dia/2+4, h/2]) cylinder(h=h+2, d=rod_dia, center=true, $fn=fn);
        translate([w/2, -l/2+22+rod_dia/2+4, -1]) cube([rod_dia-2,l-22-rod_dia/2-4,h+2]);
    translate([6, l/2-7, h/2]) rotate([0,90,0]) cylinder(h=w+w+10+2, d=screw_dia, center=true, $fn=fn);
    translate([-w/2, l/2-7, h/2]) rotate([0,90,0]) cylinder(h=w, d=6.3, center=true, $fn=6);
}

