rotate([180,0,0]){
translate([0,0,600])cube([1000,1000,10] ,center = true);
translate([0,240,0]){ 
//os
rotate([90,0,0]) {cylinder(h = 340, r1 = 30, r2 = 30, center = false);}}
translate([0,220,-20])cylinder(h = 600, r1 = 30, r2 = 30, center = false);
//zobnik/mlin?
for (a =[0:17:340]){
rotate(a=[0,a,0]) {
for (a =[0:17:340]){
translate(v = [0, 0, a]){
union(){
for (a =[0:17:170]){//faktor večanja je vedo
translate(v = [0, a, 0]){    //17
    union(){
translate(v = [0, 10, 0]) {cube(size = [12,3,12], center = false);}
translate(v = [0, 0, 0]) {cube(size = [4,12,12], center = false);}
cube(size = [12,3,12], center = false);
translate(v = [12, 0, 0]) {cube(size = [4,13,12], center = false);}
}}};

for (a =[0:17:170]){//faktor večanja je vedo
translate(v = [170, a, 0]){    //17
    union(){
translate(v = [0, 10, 0]) {cube(size = [12,3,12], center = false);}
translate(v = [0, 0, 0]) {cube(size = [4,12,12], center = false);}
cube(size = [12,3,12], center = false);
translate(v = [12, 0, 0]) {cube(size = [4,13,12], center = false);}
}}};

for (a =[0:17:170]){//faktor večanja je vedo
translate(v = [a, 170, 0]){    //17
    union(){
translate(v = [0, 10, 0]) {cube(size = [12,3,12], center = false);}
translate(v = [0, 0, 0]) {cube(size = [4,12,12], center = false);}
cube(size = [12,3,12], center = false);
translate(v = [12, 0, 0]) {cube(size = [4,13,12], center = false);}
}}};


for (a =[0:17:170]){//faktor večanja je vedo
translate(v = [a, -17, 0]){    //17
    union(){
translate(v = [0, 10, 0]) {cube(size = [12,3,12], center = false);}
translate(v = [0, 0, 0]) {cube(size = [4,12,12], center = false);}
cube(size = [12,3,12], center = false);
translate(v = [12, 0, 0]) {cube(size = [4,13,12], center = false);}
}}};}}}}

}

}
/*
//Zidak
union(){
translate(v = [0, 10, 0]) {cube(size = [12,3,12], center = false);}
translate(v = [0, 0, 0]) {cube(size = [4,12,12], center = false);}
cube(size = [12,3,12], center = false);
translate(v = [12, 0, 0]) {cube(size = [4,13,12], center = false);}
}
*/