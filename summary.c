팀블로그 -> http://blog.cloudneta.net
팀 지원 이메일 -> support@cloudneta.net

<1> AWS Infra

  1.AWS 소개

  2.AWS Network 소개
  
  3.실습전 사전 준비 사항
  	> ID 생성
	> 서비스 -> EC2 -> 네트워크 및 보안 -> 키페어 -> 키페어 생성(*.pem)
	> chmod 400 webServer.pem
	> sudo ssh -i webServer.pem ec2-user@3.35.216.71
  
  4.[실습]EC2 배포 및 사용
  	> 서비스 -> EC2 -> 인스턴스 -> 인스턴스 시작 
	> AMI 선택 -> Amazon Linux 2 AMI
	> 인스턴스 유형 -> t2.micro
	> 인스턴스 세부 정보 -> 네크워크/서브넷: 기본값, 퍼블릭 IP 자동 할당: 활성화, 나머지는 기본값
	> 스토리지 추가 -> 기본값
	> 태그 추가 -> [태그 추가], 키: Name, 값:WebServer
	> 보안 그룹 -> SSH(내 IP), 규칙 추가, HTTP(위치 무관)
	> 검토 및 시작 -> 시작하기 -> 키페어 선택 -> 바로 밑 체크 -> 인스턴스 시작 

  4-1.웹서버 설치 
	> chmod 400 webServer.pem
	> sudo ssh -i webServer.pem ec2-user@3.35.216.71
	> sudo su -
	> yum install httpd -y
	> systemctl start httpd
	> echo "<h1>Test Web Server</h1>" > /var/www/html/index.html
	> curl localhost -> 웹페이지 확인

  4-2.인스턴스 삭제
  	> 서비스 -> EC2 -> 인스턴스 -> 인스턴스 선택 -> 인스턴스 상태 -> 인스턴스 종료
  
  5.[실습]CloudFormation 스택 생성 및 삭제
	> download(1_lab2.yaml) -> http://bit.ly/cnbl0102
	> 서비스 -> CloudFormation -> 스택 생성
	> 준비된 템플릿 -> 템플릿 파일 업로드 -> 파일선택 -> 다음
	> 스택 이름(마음대로) -> KeyName(키페어 선택) -> 다음 -> 다음 -> 다음
	> 삭제 : 서비스 -> CloudFormation -> 스택 -> 생성한 스택 선택 -> 삭제(인스턴스까지 삭제됨)

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

<3> VPC 고급

  1.VPC 엔드포인트
  	> 엔드포인트
		-> 게이트웨이 엔드포인트(S3, DynamoDB 연결)
		-> 인터페이스 엔드포인트(나머지 AWS 서비스)
	> 엔드포인트 서비스 -> 사용자가 지정한 서비스 대상에 대한 프라이빗 연결
	
  2.[실습3-1] 게이트웨이/인터페이스 엔드포인트 비교

  	> 자원생성
		-> download http://bit.ly/cnbl0301
		-> VPC, 인터넷게이트웨이생성, 퍼블릭 & 프라이빗 서브넷, 라우팅테이블, EC2 생성 
		-> 서비스 -> CloudFormation -> 스택
		-> 스택생성
			-> 준비된 템플릿 -> 템플릿 파일 업로드 -> 파일선택(3_lab1.yaml) 
			-> 스택이름(CloudNeta-Lab3-1) -> KeyName(키페어선택)
	
	> 기본 환경 검증
		-> Public EC2 ssh 터미널에서 -> ping s3.ap-northeast-2.amazonaws.com -> 성공확인
		-> Private EC2 ssh 터미널에서 -> ping s3.ap-northeast-2.amazonaws.com -> 실패확인
	
	> 게이트웨이 엔드포인트생성
		-> 서비스 -> VPC -> 가상 프라이빗 클라우드 -> 엔드포인트
		-> 엔드포인트 생성
			-> search창에 s3검색 -> com.amazonaws.ap-northeast-2.s3선택(유형: Gateway)
			-> VPC(CloudNeta-VPC 선택) -> 퍼블릭 & 프라이빗 라우팅테이블 선택
			-> 라우팅 테이블에 S3 서비스에 대한 경로 생성확인
	
	> 게이트웨이 엔드포인트 검증	
		-> Public EC2 ssh 터미널에서 -> ping s3.ap-northeast-2.amazonaws.com -> 성공확인
		-> Private EC2 ssh 터미널에서 -> ping s3.ap-northeast-2.amazonaws.com -> 성공확인
  	
	> 현재 환경 동작 확인
		-> 기본 DNS 호스트 : cloudformation.ap-northeast-2.amazonaws.com
		-> Public EC2 : dig +short cloudformation.ap-northeast-2.amazonaws.com -> 52.95.193.83
		-> Private EC2 : dig +short cloudformation.ap-northeast-2.amazonaws.com -> 52.95.193.83
	
	> DNS 호스트 이름 활성화
		-> 서비스 -> VPC -> 가상프라이빗클라우드 -> VPC -> 작업
		-> DNS 호스트이름 편집 -> DNS 호스트 이름 활성화 체크   
	
	> 인터페이스 엔드포인트 생성
		-> 서비스 -> VPC -> 가상 프라이빗 클라우드 -> 엔드포인트
		-> 엔드포인트 생성
			-> search창에 cloudformation검색
			-> com.amazonaws.ap-northeast-2.cloudformation선택(유형: Interface)
			-> VPC(CloudNeta-VPC 선택) -> 프라이빗 라우팅테이블 선택
			-> 프라이빗 DNS 이름 활성화 체크
		-> 엔드포인트 생성확인 -> DNS이름 확인(vpce-xxxxxxxxxxx.vpce.amazonaws.com)
	
	> 인터페이스 엔드포인트 검증
		-> Public EC2 ssh : 
			dig +short cloudformation.ap-northeast-2.amazonaws.com -> 10.0.1.50
			dig +short vpce-xxxxxxxx.vpce.amazonaws.com -> 10.0.1.50
		-> Private EC2 ssh :
			dig +short cloudformation.ap-northeast-2.amazonaws.com -> 10.0.1.50
			dig +short vpce-xxxxxxxx.vpce.amazonaws.com -> 10.0.1.50
	
	> 자원삭제
		-> 엔드포인트 삭제 : VPC -> 엔드포인트 -> 작업 -> 엔드포인트삭제(대기시간필요)
		-> CloudFromation스택 삭제 : CloudFromation -> 스택 -> 스택삭제(대기시간 & 삭제확인) 	
  
  3.[실습3-2] 엔드포인트 서비스로 프라이빗 링크 구성
  	
	> 자원생성
  		-> download http://bit.ly/cnbl0302
		-> MyVPC : My-Public-SN, My-Public-RT, My-IGW, My-EC2  	
		   CustomVPC : Custom-Public-SN, Custom-Public-RT, Custom-IGW, Custom-WEB-1, Custom-WEB-2
			       Custom-NLB(네트웍 로드밸런서) : 타겟그룹 -> Custom-WEB-1, Custom-WEB-2
		   WebSG(보안그룹)
		-> 서비스 -> CloudFromation -> 스택 -> 스택생성
	
	> 기본환경 검증
		-> 서비스 -> EC2 -> 로드밸런싱 -> 대상그룹 -> 대상그룹선택
	      	-> 대상 탭 -> 상태 healthy 될때까지 기다림
		-> 서비스 -> EC2 -> 로드밸런싱 -> 로드밸런서선택 -> 설명탭 -> DNS이름 복사
		-> My-EC2 ssh : dig +short DNS이름 -> 15.165.14.143
				curl DNS이름 -> <h1>EndPoint Service Lab - CloudNeta Web Server 1</h1>
	
	> 엔드포인트서비스 생성 및 연결(NLB 연결)
		-> 서비스 -> VPC -> 가상 프라이빗 클라우드 -> 엔드포인트 서비스
		-> 엔드포인트서비스 생성 -> Custom-NLB선택, 태그(키:Name, 값:Custom-EPS)
	
	> 생성한 엔드포인트서비스 확인 -> 세부정보탭 -> 서비스이름 복사
	
	> 인터페이스 엔드포인트 생성 및 연결(MyVPC에 생성)
		-> 서비스 -> VPC -> 가상 프라이빗 클라우드 -> 엔드포인트
		-> 엔드포인트생성 -> 이름별서비스찾기 선택, 서비스이름(엔드포인트서비스이름 입력)->확인,
				     VPC(MyVPC 선택), 서브넷(My-Public-SN 선택), 보안그룹(WebSG 선택)
	
	> 생성한 인터페이스 엔드포인트 확인
		-> 세부정보탭 : 상태(수락대기중 확인), DNS이름(2개의 DNS이름 생성)
	
	> 엔드포인트서비스에서 엔드포인트 연결요청 수락
		-> 생성한 엔드포인트 선택
		-> 엔드포인트 연결 탭 -> 엔드포인트선택 -> 작업 -> 엔드포인트연결요청수락(팝업창 수락버튼)
		-> 2분후 사용가능상태 확인 	
	
	> 엔드포인트서비스 검증
		-> 생성한 엔드포인트 선택 -> DNS이름 복사(2개중 아무거나)
       		-> My-EC2 ssh : dig +short DNS이름 -> 10.0.0.240
				curl DNS이름 -> <h1>EndPoint Sevice Lab - CloudNeta Web Server 2</h1>
	
	> 자원삭제
		-> 엔드포인트 삭제 : VPC -> 엔드포인트 -> 작업 -> 엔드포인트삭제(대기시간)
		-> 엔드포인트서비스 삭제 : VPC -> 엔드포인트서비스 -> 작업 -> 삭제
		-> CloudFormation스택 삭제 : 대기시간 & 삭제확인  	

  4.배치그룹(Placement Group)
	> 물리적 배치

  5.메타데이터(Metadata - 속성데이터)
	> EC2 생성 & ssh terminal : curl http://169.254.169.254/latest/meta-data
				    curl http://169.254.169.254/latest/meta-data/ami-id

<4> 인터넷 연결

  1. AWS의 인터넷 연결
  	> 인터넷 게이트웨이(Layer 3) : 하나의 Private IP와 Public IP 1:1 변환
	> NAT 디바이스(Layer 4) : 다수의 Private IP를 하나의 Public IP로 변환
		-> NAT Gateway : 고가용성
		-> NAT Instance : 소규모 트래픽
	> Proxy 인스턴스(Layer 7) : Cloud내 Proxy인스턴스에게 GET www.amazon.com 질의
		-> 환경변수설정 : export http_proxy=http://10.1.1.10:3128

  2. [실습4-1] NAT 인스턴스를 통한 인터넷연결
	
  	> 자원생성
  		-> download http://bit.ly/cnbl0401
		-> 서비스 -> CloudFromation -> 스택 -> 스택생성

		-> VPC : NATInstance-VPC1(IP:10.40.0.0/16)
		-> IGW : NATInstance-IGW1(연결:NATInstance-VPC1)
		-> Public SN : NATInstance-VPC1-subnet1(IP:10.40.1.0/24, AZ:ap-northeast-2a)
		-> Public RT : NATInstance-PublicRouteTable(연결:NATInstance-VPC1-subnet1,
						라우팅정보 : 대상 0.0.0.0/0, 타깃 NATInstance-IGW1)
		-> Private SN : NATInstance-VPC1-subnet2(IP:10.40.2.0/24, AZ:ap-northeast-2a)
		-> Private RT : NATInstance-PublicRouteTable(연결:NATInstance-VPC1-subnet2)
		-> EC2 : NAT-Instance(연결:NATInstance-VPC1-subnet1,
				privateIP:10.40.1.100 - 탄력적IP 연결, AMI:amzn-ami-vpc-nat에 포함된 AMI)
			 Private-EC2-1(연결:NATInstance-VPC1-subnet2, PrivateIP:10.40.2.101, ssh:root)
			 Private-EC2-2(연결:NATInstance-VPC1-subnet2, PrivateIP:10.40.2.102, ssh:root)
		-> 보안그룹 : VPC1-NATInstance-SecureGroup(SSH/ICMP 0.0.0.0/0, HTTP/HTTPS 10.40.0.0/16)
			      VPC1-PrivateEC2-SecureGroup(SSH 10.40.0.0/16, ICMP 0.0.0.0/0)

	> 기본환경 검증
       		-> NAT EC2 elastic IP ssh : ifconfig eth0 -> private IP
					    curl http://checkip.amazonaws.com -> public elastic IP	
		-> Private-EC2-1 ssh(root:qwe123) : curl http://checkip.amazonaws.com -> 실패확인 	
		-> Private-EC2-2 ssh(=) : curl http://checkip.amazonaws.com --connect-timeout 3

	> NAT 인스턴스 동작을 위한 스크립트 확인
       		-> NAT EC2 elastic IP ssh : cat /proc/sys/net/ipv4/ip_forward -> 1(라우팅처리 가능)
		-> sudo iptables -nL POSTROUTING -t nat -v -> iptable에 IP masquerade 확인

	> NAT 인스턴스 동작을 위한 설정
		-> 프라이빗 서브넷에 라우팅정보 추가
			-> 서비스 -> VPC -> 가상프라이빗클라우드 -> 라우팅테이블
			-> NATInstance-PrivateRouteTabe1 선택 -> 라우팅탭 선택
	       		-> 라우팅편집 : 
				라우팅추가 -> 대상(0.0.0.0/0),
				      대상(네트워크 인터페이스 선택 -> NAT-Instance eth0) -> 변경사항저장
		-> source/destination 확인 비활성화(자신이 목적지가 아니면 폐기되는게 활성)
			-> 서비스 -> EC2 -> 인스턴스 -> 인스턴스 -> NAT-Instance 선택 -> 작업
			-> 네트워킹 -> 소스/대상 확인변경 -> 소스/대상 확인 밑에 중지칸 체크 -> 저장
	
	> 프라이빗 서브넷에 위치한 인스턴스에서 외부로 통신 확인
		-> Private-EC2-1 ssh : curl http://checkip.amazonaws.com -> public elastic IP
				       ping www.google.com -> 성공확인 
       		-> NAT EC2 elastic IP ssh : sudo tcpdump -nni eth0 tcp port 80 (tcp 80 port 모니터링)
	       				     - 10.40.2.101 -> 10.40.1.100 으로 변환되는걸 확인

	> 자원삭제
		-> CloudFormation -> 스택 -> 스택삭제(대기시간 & 삭제확인)	

<5>부하분산

  1. ELB(Elastic Load Balancing)
	> 종류 
	 -> ALB(Application LB) : 웹어플리케이션(HTTP,HTTPS)에 대해 특화된 LB(Layer 7), 느림
	 -> NLB(Network LB) : TCP,UDP에 대한 네트워크기반의 분산처리제공(Layer 4), 고가용성 
	 -> CLB(Classic LB) : VPC의 예전버전인 EC2-Classic에 대한 분산처리제공(사용X)
	> 통신방식
	 -> Internet Facing LB : 인터넷을 통한 LB
	 -> Internal LB : 내부통신 

  2. [실습5-1] ALB와 NLB를 통한 로드 밴런싱
  	
  	> 자원생성
  	 -> download http://bit.ly/cnbl0501
	 -> 서비스 -> CloudFromation -> 스택 -> 스택생성
	 -> VPC : ELB-VPC(IP:10.0.0.0/16)
	 	  My-VPC(IP:20.0.0.0/16)
	 -> Public SN : ELB-Public-SN-1(IP:10.0.0.0/24, AZ:ap-northeast-2a)
	 		ELB-Public-SN-2(IP:10.0.1.0/24, AZ:ap-northeast-2c)
	 		My-Public-SN(IP:20.0.0.0/24, AZ:ap-northeast-2a)
	 -> Public RT : ELB-Public-RT(연결:ELB-Public-SN-1, ElB-Public-SN-2)
	 		My-Public-RT(연결:My-Public-SN)
	 -> Internet GW : ELB-IGW(연결:ELB-VPC)
	 		  My-IGW(연결:My-VPC)
	 -> EC2 : ELB-EC2-1(연결:ELB-Public-SN-1, 데몬:HTTP,SNMP 구성)
	 	  ELB-EC2-2(연결:ELB-Public-SN-2, 데몬:HTTP,SNMP 구성)
	 	  My-EC2(연결:My-Public-SN)
	 -> 보안그룹 : ELB-SG(프로토콜:SSH,HTTP,SNMP 대상:0.0.0.0/0)

	> 기본환경 검증
	 -> My-EC2 ssh : 
	 	curl PublicIP(ELB-EC2-1)/index.html -> <h1>ELB-EC2-1 Web Server</h1>  	 
	  	curl PublicIP(ELB-EC2-1)/dev/index.html -> <h1>ELB-EC2-1 Dev Web Page</h1> 
	 	snmpget -v2c -c PublicIP(ELB-EC2-1) 1.3.6.1.2.1.1.5.0 -> SNMPv2-...=STRING:ELB-EC2-1
	 	
		curl IP(ELB-EC2-2)/index.html -> <h1>ELB-EC2-2 Web Server</h1>  	 
	  	curl IP(ELB-EC2-2)/mgt/index.html -> <h1>ELB-EC2-2 Mgt Web Page</h1> 
	 	snmpget -v2c -c public IP(ELB-EC2-2) 1.3.6.1.2.1.1.5.0 -> SNMPv2-...=STRING:ELB-EC2-2
	
	> ALB 생성
	 -> 서비스 - EC2 -> 로드밸런싱 -> 로드밸런서
	 -> 로드밸런서 생성
	 	로드밸런서 유형선택 : Application load Balancer 선택
       		로드밸런서 구성 : 이름(ALB-TEST), 체계(Internet-facing 선택)
				  VPC(ELB-VPC 선택), Mappings(ELB-Public-SN-1, ELB-Public-SN-2 선택)
		보안그룹 구성 : Security Group(ELB-SG 선택), default SG은 삭제
		Listeners and Routing :
			Protocol(HTTP), Port(80)
			default action -> create target group
			 -> Specify group details : target type(instance), target group name(ALB-TG),
						   Protocol(HTTP), Port(80), Health checks(default)
			 -> Next click
			 -> Register targets : Available instances(ELB-EC2-1, ELB-EC2-2 선택)
					      -> include as pending below
			 -> Create Target Group click
			default action(ALB-TG 선택) 
			Create load balancer click
	 -> ALB 생성확인 : 상태(provisioning) <- 동작상태 아님
	 -> 서비스 - EC2 -> 로드밸런싱 -> 대상그룹 -> 대상탭 : 대상인스턴스의 상태(initial)
         -> 몇분후 ALB 상태 -> active, 대상 인스턴스 상태 -> healthy 로 바뀌면 성공 
	
	> ALB 검증(Round Robin이 기본 알고리즘)  
	 -> 생성한 ALB-TEST 선택 -> 설명탭 -> ALB DNS 주소복사
 	 -> My-EC2 ssh : curl DNS주소/index.html -> <h1>ELB-EC2-1 Web Server</h1>	 
 	 		 curl DNS주소/index.html -> <h1>ELB-EC2-2 Web Server</h1>	 
	
	> ALB 검증2(ELB-EC2-1에는 /dev/index.html만 있고, ELB-EC2-2에는 /mgt/index.html만 있음)
 	 -> My-EC2 ssh : curl DNS주소/dev/index.html -> <h1>ELB-EC2-1 Dev Web Page</h1>	 
 	 		 curl DNS주소/dev/index.html -> 실패	 
 	 -> My-EC2 ssh : curl DNS주소/mgt/index.html -> 실패 	 
 	 		 curl DNS주소/mgt/index.html -> <h1>ELB-EC2-2 mgt Web Page</h1>	 

	> 경로기반 라우팅 설정(검증2를 해결하기 위해)
	 -> Dev-Group 대상그룹 생성
		서비스 -> EC2 -> 로드밸런싱 -> 대상그룹
		-> 대상그룹 생성 :
			대상그룹 이름:Dev-Group, VPC:ELB-VPC
			Register targets : Available instances(ELB-EC2-1선택)
				-> include as pending below
			 	-> Create Target Group click
	 -> Mgt-Group 대상그룹 생성 : 
			위와 같이(대상그룹 이름: Mgt-Group, VPC:ELB-VPC, Avail instances:ELB-EC2-2선택)	
	 -> 생성된 대상그룹 확인(ALB-TG, Dev-Group, Mgt-Group)
	 -> Dev-Group의 조건 설정 
		생성한 ALB 선택(ALB-TEST) -> 리스너탭 -> 생성한 리스너 선택 -> 규칙보기/편집 클릭
	     	-> +선택 -> 규칙삽입 : IF -> 경로 -> 확장하는데 -> /dev/* 입력                       */
				       THEN -> 전달대상 -> 대상그룹(Dev-Group) -> 저장
	 -> Mgt-Group의 조건 설정
		위와 같이(/mgt/ , Mgt-Group)
	 -> ALB 리스너 규칙 확인(3규칙)
	
	> 경로기반 라우팅 검증
 	 -> My-EC2 ssh : curl DNS주소/dev/index.html -> <h1>ELB-EC2-1 Dev Web Page</h1>	 
 	 		 curl DNS주소/dev/index.html -> <h1>ELB-EC2-1 Dev Web Page</h1>	 
 	 -> My-EC2 ssh : curl DNS주소/mgt/index.html -> <h1>ELB-EC2-2 mgt Web Page</h1>	 
 	 		 curl DNS주소/mgt/index.html -> <h1>ELB-EC2-2 mgt Web Page</h1>	 

	> NLB 생성
	 -> 서비스 - EC2 -> 로드밸런싱 -> 로드밸런서
	 -> 로드밸런서 생성
	 	로드밸런서 유형선택 : NetWork load Balancer 선택
       		로드밸런서 구성 : 이름(NLB-TEST), 체계(Internet-facing 선택)
				  VPC(ELB-VPC 선택), Mappings(ELB-Public-SN-1, ELB-Public-SN-2 선택)
		Listeners and Routing :
			Protocol(UDP), Port(161)
			default action -> create target group
			 -> Specify group details : target type(instance), target group name(NLB-TG),
						   Protocol(UDP), Port(161),
						   Health checks(HTTP),
						   Advanced health check settings(override:80) 
			 -> Next click
			 -> Register targets : Available instances(ELB-EC2-1, ELB-EC2-2 선택)
					      -> include as pending below
			 -> Create Target Group click
			default action(NLB-TG 선택) 
			Create load balancer click
	 -> NLB 생성확인 : 상태(provisioning) <- 동작상태 아님
	 -> 서비스 - EC2 -> 로드밸런싱 -> 대상그룹 -> 대상탭 : 대상인스턴스의 상태(initial)
         -> 몇분후 NLB 상태 -> active, 대상 인스턴스 상태 -> healthy 로 바뀌면 성공 
	
	> NLB 검증(Round Robin이 기본 알고리즘)  
	 -> 생성한 NLB-TEST 선택 -> 설명탭 -> NLB DNS 주소복사
 	 -> My-EC2 ssh : snmpget -v2c -c public DNS주소 1.3.6.1.2.1.1.5.0 -> SNMPv2...STRING:ELB-EC2-1	 
 	 		 snmpget -v2c -c public DNS주소 1.3.6.1.2.1.1.5.0 -> SNMPv2...STRING:ELB-EC2-2	 

	> NLB의 특징 확인
 	 -> ELB-EC2-1 ssh : sudo tcpdump udp port 161 -nn
	 			-> IP 13.X.X.X > 10.X.X.X: GetRequest(28) OID
	 			   IP 10.X.X.X > 13.X.X.X: GetResponse(37) OID="ELB-EC2-1"
	 -> NLB는 클라이언트 IP를 바꾸지않음(인스턴스 유형), IP 유형은 바꿈
	 -> ALB는 자신의 Local IP로 바꿈 : 대신 X-Forwarded-For 헤더에 클라이언트 IP 정보를 담아서 전달
						-> 관련정보(https://amzn.to/3jhhnu2)
	
	> 자원삭제
	 -> 로드밸런서(2개) 삭제(EC2 -> 로드밸런싱 -> 로드밸런서 -> 작업 -> 삭제)	
	 -> 대상그룹(4개) 삭제(EC2 -> 로드밸런싱 -> 대상그룹 -> 작업 -> 삭제)
	 -> CloudFormation -> 스택 -> 스택삭제(대기시간 & 삭제확인)	

  3. Route 53
	> 도메인 이름 생성
	 -> 유료 : Route 53을 도메인 등록대행소로 활용 -> http://bitly.kr/cnba51 -> 2단계:도메인이름등록
	 	   서비스 -> Route 53 -> 도메인 -> 등록된 도메인 -> 도메인이름 확인
		   서비스 -> Route 53 -> 호스팅 영역 -> 호스팅 영역 -> 도메인 이름 확인  
 	 -> 무료 : 무료 DNS 서비스(ex>freenom) -> http://bit.ly/cnba0502	 
		   서비스 -> Route 53 -> 호스팅 영역 -> 호스팅 영역 -> 도메인 이름 확인  

  4. [실습5-2] Route 53 구성 및 라우팅 정책
  5. CloucFront
  6. [실습5-3] CloudFront를 통한 CDN 서버스
  7. Global Accelerator
  8. [실습5-4] 글로벌 네트워크를 활용한 Global Accelerator
