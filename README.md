# Graphics Game Programming Practice
* Game Programming in C++ 책 마지막 챕터까지의 내용을 오픈소스 라이브러리로 변경하는 프로젝트
* 2019/12 ~ 2022/01 까지 공부했던 내용

# 기존 프로젝트와의 변경점
* 기존 책의 프로젝트는 OpenGL, SDL, SOIL, RapidJson, FMOD를 사용
* 그래픽스 : OpenGL 3.3 그대로 사용
* 미디어 : SDL -> SDL2 최신버전 사용
* 수학 : 자체 작성 Math 헤더파일 -> GLM 사용
* 2D 이미지 : SOIL -> SOIL2 사용
* Mesh, 텍스쳐, 메터리얼 : RapidJson -> ASSIMP 사용
* 사운드 : FMOD -> OpenAL 사용

# 참조
* 원본 프로젝트 : https://github.com/gameprogcpp/code

# 추가 목표
* Learn OpenGL 공부해서 Graphics 내용 업그레이드
* Vulkan 라이브러리 파이프라인 적용해보기
* DirectX 라이브러리 파이프라인 적용해보기

# 작업내역
