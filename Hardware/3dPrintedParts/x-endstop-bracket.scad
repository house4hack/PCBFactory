l=37;
w=30;
h=3;
stepper_hole_dist=31;
limit_hole_dist=9.5;

fn=16;

difference(){
translate([-w/2, -l/2, 0]) cube([w,l,h]);
#translate([w/2-4, -stepper_hole_dist/2, 0]) cylinder(h=8,r=1.5, center=true, $fn=fn);
#translate([w/2-4, stepper_hole_dist/2, 0]) cylinder(h=8,r=1.5, center=true, $fn=fn);
translate([-w/2+4, -l/2+5, 0]) cylinder(h=8,r=1, center=true, $fn=fn);
translate([-w/2+4, -l/2+5+limit_hole_dist, 0]) cylinder(h=8,r=1, center=true, $fn=fn);
translate([-w+4, 0, -1]) cube([w,l,h+2]);
}