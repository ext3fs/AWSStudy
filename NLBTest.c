1. VPC 생성 -> 자동으로 라우팅테이블 생성됨 
	>> 모든 서비스 -> 네트워킹 및 콘텐츠 전송 -> VPC
	>> VPC -> 가상 프라이빗 클라우드 -> VPC
	>> VPC 생성
       		>> 이름태그 -> myVPC
		>> IPv4 CIDR -> 10.0.0.0/16
		>> VPC 생성 버튼

2. public SN1, SN2 생성
	>> 모든 서비스 -> 네트워킹 및 콘텐츠 전송 -> VPC
	>> VPC -> 가상 프라이빗 클라우드 -> 서브넷
	>> 서브넷 생성
		>> VPC ID -> myVPC 선택
		>> 서브넷이름 -> SN1
		>> 가용영역 -> 아시아태평양(서울)/ap-northeast-2a 선택
		>> IPv4 CIDR -> 10.0.0.0/24
		>> 서브넷 생성 버튼 
	>> 서브넷 생성
		>> VPC ID -> myVPC 선택
		>> 서브넷이름 -> SN2
		>> 가용영역 -> 아시아태평양(서울)/ap-northeast-2c 선택
		>> IPv4 CIDR -> 10.0.1.0/24
		>> 서브넷 생성 버튼 

3. internet gateway 생성
	>> 모든 서비스 -> 네트워킹 및 콘텐츠 전송 -> VPC
	>> VPC -> 가상 프라이빗 클라우드 -> 인터넷 게이트웨이
	>> 인터넷 게이트웨이 생성
		>> 이름태그 -> myIGW
		>> 인터넷 게이트웨이 생성 버튼
	>> 상단에 VPC에 연결 버튼 클릭 
      		>> 사용가능한 VPC -> myVPC
		>> 인터넷 게이트웨이 연결 버튼 	

4. roting table 편집
	>> 모든 서비스 -> 네트워킹 및 콘텐츠 전송 -> VPC
	>> VPC -> 가상 프라이빗 클라우드 -> 라우팅테이블
	>> myVPC에 연결된 라우팅테이블이 이미 생성되어 있음
	>> 이름을 myRT로 저장
	>> 선택후 하단에 서브넷연결 탭
		>> 서브넷 연결 편집
	       		>> 이용가능한 서브넷 SN1, SN2 선택
			>> 연결저장 버튼 
	>> 하단의 라우팅 탭
		>> 라우팅 편집
			>> 라우팅 추가 버튼
				>> 대상 : 0.0.0.0/0  대상 : 인터넷게이트웨이(myIGW) 선택	
				>> 변경사항저장 버튼			

5. SN1 & SN2에 웹서버 생성
	>> 인스턴스 시작
		>> Amazon linux 2 AMI 선택
		>> 인스턴스 세부 정보 구성
			>> 네트워크 -> myVPC
			>> 서브넷 -> SN1
			>> 퍼블릭 ip 자동할당 -> 활성화 
		>> 태그추가 -> Name : EC2-1
		>> 보안그룹 구성
			>> 새보안그룹 생성 -> SSH : 내 IP, HTTP : 위치무관	
		>> 생성 
	>> 인스턴스 시작
		>> Amazon linux 2 AMI 선택
		>> 인스턴스 세부 정보 구성
			>> 네트워크 -> myVPC
			>> 서브넷 -> SN2
			>> 퍼블릭 ip 자동할당 -> 활성화 
		>> 태그추가 -> Name : EC2-2
		>> 보안그룹 구성
			>> 바로 위의 보안그룹 재사용 -> SSH : 내 IP, HTTP : 위치무관	
		>> 생성 

	>> EC2-1, EC2-2 에 웹서버 설치

6. NLB 생성

*삭제* 
1. NLB 삭제
2. target group 삭제
3. EC2-1 & EC2-2 삭제
4. VPC 삭제 