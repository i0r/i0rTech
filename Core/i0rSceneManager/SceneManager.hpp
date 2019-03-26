#pragma once

class SceneManager : public ISceneManager {
	public:
		explicit SceneManager() ;
		~SceneManager() ;

		void Create( const bool displayEditorGrid = false ) ;
		void Clear() ;
		void OnTick() ;
		void ComputeVisibility() ;

		void AddNode( Mesh* mesh ) ;
		void CreateLocalActor() ;

	private:
		void CreateGridNode() ;
};