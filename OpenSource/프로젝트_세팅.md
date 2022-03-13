## 프로젝트 세팅
* 아래와 같이 프로젝트 세팅하기
* 프로젝트 우클릭 -> 속성
* C/C++ 
- 항목 : 일반 -> 추가 포함 디렉터리
- 아래 내용 복사 후 붙여넣기
..\Libx64\GLEW\include;..\Libx64\ASSIMP\include;..\Libx64\GLM\include;..\Libx64\SOIL2\include;..\Libx64\SDL2\include;%(AdditionalIncludeDirectories)

- 항목 : 전처리기 -> 전처리기 정의
- 아래 내용 복사 후 붙여넣기
_DEBUG;_CONSOLE;_LIB;%(PreprocessorDefinitions)

* 링커
- 항목 : 일반 -> 추가 라이브러리 디렉터리
- 아래 내용 복사 후 붙여넣기
..\Libx64\GLEW\lib;..\Libx64\ASSIMP\lib;..\Libx64\GLM\lib;..\Libx64\SOIL2\lib;..\Libx64\SDL2\lib;%(AdditionalLibraryDirectories)

- 항목 : 입력 -> 추가 종속성
- 아래 내용 복사 후 붙여넣기
opengl32.lib;glew32.lib;assimp-vc143-mt.lib;glm.lib;SOIL2.lib;SDL2.lib;SDL2main.lib;SDL2_ttf.lib;%(AdditionalDependencies)

* 빌드 이벤트 
- 항목 : 빌드 후 이벤트 -> 오른쪽 끝 화살표 -> 편집
- 아래 내용 복사 후 붙여넣기
xcopy "$(ProjectDir)\..\Libx64\GLEW\lib\*.dll" "$(OutDir)" /i /s /y
xcopy "$(ProjectDir)\..\Libx64\ASSIMP\lib\*.dll" "$(OutDir)" /i /s /y
xcopy "$(ProjectDir)\..\Libx64\SDL2\lib\*.dll" "$(OutDir)" /i /s /y