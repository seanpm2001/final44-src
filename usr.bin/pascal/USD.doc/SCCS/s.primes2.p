h02961
s 00000/00000/00030
d D 8.1 93/06/08 18:09:30 bostic 2 1
c 4.4BSD snapshot (revision 8.1)
e
s 00030/00000/00000
d D 5.1 90/06/07 16:14:49 bostic 1 0
c date and time created 90/06/07 16:14:49 by bostic
e
u
U
t
T
I 1
program primes(output);
const n = 50; n1 = 6; (*n1 = sqrt(n)*)
var i,k,x,inc,lim,square,l: integer;
    prim: boolean;
    p,v: array[1..n1] of integer;
begin
   write(2:6, 3:6); l := 2;
   x := 1; inc := 4; lim := 1; square := 9;
   for i := 3 to n do
   begin (*find next prime*)
      repeat x := x + inc; inc := 6-inc;
         if square <= x then
            begin lim := lim+1;
               v[lim] := square; square := sqr(p[lim+1])
            end ;
         k := 2; prim := true;
         while prim and (k<lim) do
         begin k := k+1;
            if v[k] < x then v[k] := v[k] + 2*p[k];
            prim := x <> v[k]
         end
      until prim;
      if i <= n1 then p[i] := x;
      write(x:6); l := l+1;
      if l = 10 then
         begin writeln; l := 0
         end
   end ;
   writeln;
end .
E 1
