//#ifndef mdiWidget_hpp
//#define mdiWidget_hpp
//
//#include <axLib/axXml.h>
//#include <axLib/axLib.h>
//#include <axWindowManager.h"
//
//namespace mdi {
//namespace ctrl {
//	void SetupControl(ax::Window* win);
//	
//	class Builder {
//	public:
//		Builder()
//		{
//		}
//
//		virtual std::shared_ptr<ax::Window::Backbone> Create(
//			const ax::Point& pos, const std::string& file_path)
//			= 0;
//		
//		virtual std::shared_ptr<ax::Window::Backbone> Create(rapidxml::xml_node<>* node)
//		{
//			return nullptr;
//		};
//	};
//
//	class Control : public ax::Utils::Component {
//	public:
//		typedef std::shared_ptr<Control> Ptr;
//
//		Control(ax::Window* win, const std::string& name)
//			: _win(win), _name(name)
//		{
//		}
//
//		std::string GetName() const
//		{
//			return _name;
//		}
//		
//		virtual void Save(rapidxml::xml_node<>* node) = 0;
//		
//		ax::Window* GetWindow()
//		{
//			return _win;
//		}
//		
//	private:
//		ax::Window* _win;
//		std::string _name;
//		
//	};
//}
//}
//
//#endif // mdiWidget_hpp
