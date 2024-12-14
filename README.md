# GAFinalProject
Game Institute - Final Project   
**Engine** : Paragon Engine   
**Game** : Stolen Youth   

## 특징 & 기술 스택
- 자체 엔진 활용 개발 (로직, 그래픽스, 물리)
- Direct3D 11 기반 렌더링 엔진
  - Deferred Rendering
  - Skinning Animation
  - GPU Instancing
  - PBR, IBL
  - Alpha Blending, Alpha Clipping
  - Shadow Mapping, PCF Shadow Filtering
- D3D11 셰이더 리플렉션 기반 자체 셰이더 매터리얼 시스템 및 에디터 (PgShaderParser)
- 언리얼 맵 데이터 + 콜라이더 정보 파싱 및 이식
- 유니티 라이트맵 데이터 이식, Mixed Lighting 구현 (리얼타임 + 유니티 라이트맵)
- C++ Scripting & Compile Time Reflection
- PhysX 활용 물리엔진
- FMOD 사운드 시스템
- 자체 자료양식 (라이트맵, 애니메이션, 매터리얼, 씬)
- IMGUI 활용 에디터 (씬 세이브, 로드, 편집 등등)
- BehaviorTree.CPP 활용, 행동 트리 로직.

## 빌드 파일 
20240726_FinalRelease.zip   
https://drive.google.com/file/d/1rME4_pxHhvJAIlWs5VfaI4h3jok3q8Hj/view?usp=sharing   

## 리소스 / 라이브러리 연동
https://drive.google.com/file/d/1PuR2Y16n6jOeUJ77kqd0DlvvAwhFV7nd/view?usp=sharing
1. ParagonDistribution.zip을 풀어주세요.
2. 내부 "lib" 폴더를 GAFinalProject/ParagonEngine/lib 경로가 유효하게 옮겨주세요.
3. 내부 "Resources" 폴더를 GAFinalProject/ParagonEngine/Resources 경로가 유효하게 옮겨주세요.
4. 솔루션을 빌드해주세요.

## 개발 기간
23/09/26 - 24/07/25 (10개월)

## 팀원
기획 1명 / 아트 3명 / 프로그래머 3명   
[고태욱](https://github.com/TaewookWorks)
[변지상](https://github.com/byeonjisang)
[최민서](https://github.com/Min-seo-Choi)

프로그래머 (~24/01)
[김정식](https://github.com/jimangseng)

## 게임 설명
액션, 투사체 기반, 쿼터뷰, 보스 대전

## More Info
PreviousGitHistory에 기존 커밋 히스토리 있습니다!
