lmh_w=20;
lmh_l=40;
lmh_d=3;
lmh_h=5;

b_rim=5;
b_thick=4;
bz_w=lmh_w+b_rim+b_rim;
bz_l=lmh_l+b_rim+b_rim;
by=27.5+b_thick;
bx=bz_w;
bmh_d=5;
bmh_w=15;

fr=27.5;
fn=16;

rotate([-90,0,0]) {
    //Sample alu profile
    //#translate([-100, -fr/2, -fr]) cube([200,fr,fr]);

    //alu profile base
    translate([-bx/2, -(by-b_thick)/2, 0]) difference() {
        cube([bx,by,b_thick]);
        #translate([bx/2-(bmh_w/2), (by-b_thick)/2, b_thick/2]) cylinder(h=b_thick+2,d=bmh_d, center=true, $fn=fn);
        #translate([bx/2+(bmh_w/2), (by-b_thick)/2, b_thick/2]) cylinder(h=b_thick+2,d=bmh_d, center=true, $fn=fn);
    }

    //laser base
    translate([-bx/2, 27.5/2+b_thick, 0]) rotate([90,0,0])
    difference() {
        cube([bz_w,bz_l+lmh_h,b_thick]);
        #translate([bz_w/2-(lmh_w/2), bz_l/2-(lmh_l/2)+lmh_h, -1]) cylinder(h=b_thick+2,d=lmh_d, $fn=fn);
        #translate([bz_w/2-(lmh_w/2), bz_l/2+(lmh_l/2)+lmh_h, -1]) cylinder(h=b_thick+2,d=lmh_d, $fn=fn);
        #translate([bz_w/2+(lmh_w/2), bz_l/2-(lmh_l/2)+lmh_h, -1]) cylinder(h=b_thick+2,d=lmh_d, $fn=fn);
        #translate([bz_w/2+(lmh_w/2), bz_l/2+(lmh_l/2)+lmh_h, -1]) cylinder(h=b_thick+2,d=lmh_d, $fn=fn);
    }

}