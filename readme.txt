[]
- mt.exe, signtool.exe, manifest 파일들은 AnySupport, LinkMeMine, HelpU 등의 제품들에 대해 공통인지?
- 지문은 제품마다 틀리지만 실행 파라미터는 동일한지?

- 

- 좌측에 제품 선택 트리
- 우측에 설정 및 "CodeSign with manifest", "CodeSign without manifest" 등 액션 영역 추가
	서비스 강제 종료
	CodeSign 툴 루트 경로가 정해지면 아래 3개의 경로는 자동 결정됨.
		mttool path
		signtool path
		manifest path
- 지문
- 

[저장방식]
- ini? registry?

[registry]
- KoinoTools/LinkMeMine/
	- mttool path
	- signtool path
	- manifest path
