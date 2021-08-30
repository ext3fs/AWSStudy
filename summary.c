팀블로그 -> http://blog.cloudneta.net
팀 지원 이메일 -> support@cloudneta.net

<1> AWS Infra
  1. AWS 소개

  2. AWS Network 소개
  
  3. 실습전 사전 준비 사항
  	> ID 생성
	> 서비스 -> EC2 -> 네트워크 및 보안 -> 키페어 -> 키페어 생성(pem)
	> chmod 400 webServer.pem
	> sudo ssh -i webServer.pem ec2-user@3.35.216.71
  
  4.[실습]EC2 배포 및 사용
  	> 서비스 -> EC2 -> 인스턴스 -> 인스턴스 시작 
	> AMI 선택 -> Amazon Linux 2 AMI
	> 인스턴스 유형 -> t2.micro
	> 인스턴스 세부 정보 -> 네크워크/서브넷: 기본값, 퍼블릭 IP 자동 할당: 활성화, 나머지는 기본값
	> 스토리지 추가 -> 기본값
	> 태그 추가 -> [태그 추가], 키: Nmae, 값:WebServer
	> 보안 그룹 -> SSH(내 IP), 규칙 추가, HTTP(위치 무관)
	> 키 페어 선택

  4-1.웹서버 설치 
	> chmod 400 webServer.pem
	> sudo ssh -i webServer.pem ec2-user@3.35.216.71
	> sudo su -
	> yum install httpd -y
	> systemctl start httpd
	> echo "<h1>Test Web Server</h1>" > /var/www/html/index.html
	> curl localhost -> 웹페이지 확인

  4-2.인스턴스 삭제
  	> 서비스 -> EC2 -> 인스턴스 -> 인스턴스 선택 -> 작업 -> 인스턴스 상태 -> 종료
  
  5.[실습]CloudFormation 스택 생성 및 삭제
	> https://bit.ly/cnblo1o2 -> 1_lab2.yaml 다운로드
	> 서비스 -> CloudFormation -> 스택 생성
	> 준비된 템플릿 -> 템플릿 파일 업로드 -> 파일선택 -> 다음
	> 스택 이름(마음대로) -> KeyName(키페어 선택) -> 다음 -> 다음 -> 다음
	> 삭제 : 서비스 -> CloudFormation -> 스택 -> 생성한 스택 선택 -> 스택 삭제(인스턴스까지 삭제됨)

<2> VPC 기초
  1.VPC(Virtual Private Cloud)
  
  2.기본 네트워크 개념 이해

  3.VPC 리소스 소개
  	> VPC 생성 -> VPC + 가상라우터 + 기본 라우팅테이블 자동생성 

  4.[실습]퍼블릭 서브넷 VPC 구성
  	> 서비스 -> VPC -> 가상 프라이빗 클라우드 -> VPC
	> VPC생성 -> 이름태그(CloudNeta-VPC), IPv4 CIDR 블록(10.0.0.0/16)  
	> 서비스 -> VPC -> 가상 프라이빗 클라우드 -> 서브넷
	> 서브넷생성 -> 이름태그(CloudNeta-Public-SN), VPC(CloudNeta-VPC 선택), 
	                가용영역(서울ap-northeast-2a), IPv4 CIDR 블록(10.0.0.0/24)
  	> 서비스 -> VPC -> 가상 프라이빗 클라우드 -> 인터넷 게이트웨이
	> 인터넷 게이트웨이 생성 -> 이름태그(CloudNeta-IGW)
  	> 인터넷 게이트웨이 -> 작업 -> VPC에 연결 -> 사용가능한 VPC(CloudNeta-VPC 선택)
  	> 서비스 -> VPC -> 가상 프라이빗 클라우드 -> 라우팅테이블
	> 라우팅테이블 생성 -> 이름태그(CloudNeta-Public-RT), VPC(CloudNeta-VPC 선택)
	> 라우팅테이블 -> 서브넷연결 탭 -> 서브넷연결 편집 -> 서브넷ID(CloudNeta-Public-SN 선택)
	> 라우팅테이블 -> 라우팅 탭 -> 라우팅 편집 -> 라우팅추가 -> 대상(0.0.0.0/0), 대상(CloudNeta-IGW)
	> 서비스 -> EC2 -> 인스턴스 -> 인스턴스 
	> 인스턴스 시작 
		-> AMI 선택 -> Amazon Linux 2 AMI
		-> 인스턴스 유형 -> t2.micro
		-> 인스턴스 세부정보구성 -> 네트워크(CloudNeta-VPC),
		                            서브넷(CloudNeta-Public-SN), 퍼블릭IP 자동할당(활성화)
		-> 스토리지 추가 -> default
		-> 태그추가 -> 키(Name), 값(Public-EC2)
		-> 보안그룹 설정 -> default
		-> 키페어 선택
	> ssh -i webServer.pem ec2-user@3.35.216.71 -> ping google.com 성공확인

  5.[실습]프라이빗 서브넷 VPC 구성
  	> 서비스 -> VPC -> 가상 프라이빗 클라우드 -> 서브넷
	> 서브넷 생성 -> 이름태그(CloudNeta-Private-SN), VPC(CloudNeta-VPC 선택),
	                 가용영역(서울ap-northeast-2c), IPv4 CIDR 블록(10.0.1.0/24)
  	> 서비스 -> VPC -> 가상 프라이빗 클라우드 -> NAT 게이트웨이
	> NAT 게이트웨이 생성 -> 서브넷(CloudNeta-Public-SN), 새 EIP생성 클릭,
				 키와값 설정(Name, CloudNeta-NAT)
  	> 서비스 -> VPC -> 가상 프라이빗 클라우드 -> 라우팅테이블
	> 라우팅테이블 생성 -> 이름태그(CloudNeta-Pravate-RT), VPC(CloudNeta-VPC 선택)
	> 라우팅테이블 -> 서브넷연결 탭 -> 서브넷연결 편집 -> 서브넷ID(CloudNeta-Private-SN 선택)
	> 라우팅테이블 -> 라우팅 탭 -> 라우팅 편집 -> 라우팅추가 -> 대상(0.0.0.0/0), 대상(CloudNeta-NAT)
	> 서비스 -> EC2 -> 인스턴스 -> 인스턴스
	> 인스턴스 시작 
		-> AMI 선택 -> Amazon Linux 2 AMI
		-> 인스턴스 유형 -> t2.micro
		-> 인스턴스 세부정보 -> 네크워크(CloudNeta-VPC), 서브넷(CloudNeta-Private-SN),
					나머지는 기본값, http://bit.ly/cnbl0202 다운로드
					하단의 고급세부정보
						-> 사용자 데이터 -> cnbl0202 -> 텍스트로 붙여넣기 
		-> 스토리지 추가 -> default
		-> 태그추가 -> 키(Name), 값(Private-EC2)
		-> 보안그룹 설정 -> default
		-> 키페어 선택
	> Public-EC2로 ssh 접속
	> Private-EC2로 ssh 접속(ssh root@10.0.1.242 -> qwe123)
	> ping google.com 성공확인

  5-3.자원삭제
	> EC2 인스턴스 종료(EC2 -> 인스턴스 -> 인스턴스선택 -> 작업 -> 인스턴스상태 -> 종료)
	> NAT 게이트웨이삭제(VPC -> NAT 게이트웨이 -> 작업 -> NAT 게이트웨이삭제 ) -> 삭제될때까지 기다림
	> 탄력적 IP삭제(VPC -> 탄력적 IP -> Actions -> 탄력적 IP 주소 릴리즈) 	
	> VPC 삭제(VPC -> VPC -> 작업 -> VPC 삭제)



















