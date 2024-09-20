program sumNumbers 
implicit none  
   integer( kind = 8) :: i, n = 100000000
   integer( kind = 8) :: s = 0
   INTEGER (kind = 8) :: count1, count2, countRate, countRateNs, elapsedNs 
  
   CALL SYSTEM_CLOCK(count1, countRate)
   ! s = (n/2)*(n+1)
    do i = 0, n 
      s = i + s
   end do 
   
   CALL SYSTEM_CLOCK(count2)
   countRateNs = countRate/1000000000
   elapsedNs = (count2 - count1)
   print*,  s, elapsedNs 
end program sumNumbers  
