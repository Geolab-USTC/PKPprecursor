      program tcurve
      save
      parameter (max=60, nnphase=120,nax=180)
      logical log,prnt(3)
      character*8 phcd(max),phlst(10)
      character*256 modnam
      dimension tt(max),dtdd(max),dtdh(max),dddp(max),mn(max),ts(max)
      dimension usrc(2)

      dimension tt0(nnphase,nax), deltas(nax) 
      character*8 phcd0(nnphase)

      data in/1/,modnam/'/usr/local/local/ttimes/iasp91'/
      data phlst(1)/'query'/,prnt(3)/.true./

c
      write(6,*) 'This routine for calculating travel time curve for'
      write(6,*) 'for the iasp91 model stored as iasp91.hed, iasp91.tbl'
      write(6,*)
      prnt(1) = .false.
      prnt(2) = .false.
      call assign(10,2,'ttim1.lis')
      call tabin(in,modnam)
      write(6,*) 'The source depth has to be specified and also'
      write(6,*) 'the phase codes or keywords for the required branches'
      write(6,*) 'ALL will give all available branches'
      write(6,*) 'P  gives P-up,P,Pdiff,PKP, and PKiKP'
      write(6,*) 'P+ gives P-up,P,Pdiff,PKP,PKiKP,PcP,pP,pPdiff,pPKP,' 
      write(6,*) '         pPKiKP,sP,sPdiff,sPKP, and sPKiKP'
      write(6,*) 'S  gives S-up,S,Sdiff, and SKS'
      write(6,*) 'S+ gives S-up,S,Sdiff,SKS,sS,sSdiff,sSKS,pS,pSdiff,'
      write(6,*) '         and pSKS '
      write(6,*) 'basic gives P+ and S+ as well as '
      write(6,*) '         ScP, SKP, PKKP, SKKP, PP, and PKPPKP '
      write(6,*)
      write(6,*) 'or give a generic phase name'
      write(6,*)
      call brnset(1,phlst,prnt)
c                                    choose source depth
 3    call query('Source depth (km):',log)
      read(*,*)zs
      if(zs.lt.0.) then 
           write(6,*) 'Source depth can not be < 0'
           go to 3
      endif

      call depset(zs,usrc)

c     loop on delta
      nphase = 0
      do jdist=1,180
          delta = jdist
          deltas(jdist) = delta
          call trtm(delta,max,n,tt,dtdd,dtdh,dddp,phcd)
          do i=1,n
c             /* search out the branch */
              index = index0(phcd(i),nphase,phcd0)
              tt0(index,jdist) = tt(i)
          enddo
      enddo

c     /* output the delta-times curve */
      open(11,file="tcurve.out")
      write(11,101)"dist",(' ', phcd0(i),i=1,nphase)
      do jdist=1,180
          write(11,100)deltas(jdist),(' ',tt0(i,jdist),i=1,nphase)
      enddo
      close(11)
 100  format(1x,f6.2,120(a1,f8.2))
 101  format(1x,a6,120(a1,a8))

 13   call retrns(in)
      call retrns(10)
      call exit(0)
      end

      function index0(ph,nphase,phcd0)
      parameter (max=120)

      character*8 phcd0(max)
      character*8 ph
      
      index = 0
      do i=1,nphase
          if(phcd0(i) .eq. ph) index = i
      enddo

      if(index .eq. 0)then
          nphase = nphase + 1
          index  = nphase
          phcd0(index) = ph
      endif

      index0 = index
      end

      
          
