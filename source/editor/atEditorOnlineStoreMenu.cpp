#include "editor/atEditorOnlineStoreMenu.hpp"

#include <boost/filesystem.hpp>
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>

namespace at {
namespace editor {
	OnlineStore::OnlineStore(ax::event::Object* obj)
		: _obj(obj)
	{
	}

	//	void OnlineStore::DownloadWidgetData()
	//	{
	//		std::string file_path("server_widget_resources/test.txt");
	//
	//		// Path already exist.
	//		if (boost::filesystem::exists(file_path)) {
	//			// Check date of some shit before downloading.
	//			ax::console::Print("File :", file_path, "alredy exist.");
	//			ax::console::Print("Need to check for file validity and check date before redownload.");
	//			return;
	//		}
	//
	//		auto fileStream = std::make_shared<concurrency::streams::ostream>();
	//
	//		ax::event::Object* obj = _obj;
	//
	//		// Open stream to output file.
	//		pplx::task<void> requestTask
	//			= concurrency::streams::fstream::open_ostream(file_path)
	//				  .then([=](concurrency::streams::ostream outFile) {
	//					  *fileStream = outFile;
	//
	//					  // Create http_client to send the request.
	//					  web::http::client::http_client
	// client(U("http://audiotools-audiotools.rhcloud.com/"));
	//
	//					  // Build request URI and start the request.
	//					  web::http::uri_builder builder(U("/widgets"));
	//					  // builder.append_query(U("q"), U("Casablanca CodePlex"));
	//
	//					  return client.request(web::http::methods::GET, builder.to_string());
	//				  })
	//
	//				  // Handle response headers arriving.
	//				  .then([=](web::http::http_response response) {
	//					  printf("Received response status code:%u\n", response.status_code());
	//
	//					  // Write response body into the file.
	//					  return response.body().read_to_end(fileStream->streambuf());
	//				  })
	//
	//				  // Close the file stream.
	//				  .then([=](size_t) {
	//					  obj->PushEvent(10, new ax::event::EmptyMsg());
	//					  return fileStream->close();
	//				  });
	//	}
	//
	//	pplx::task<void> RequestJSONValueAsync()
	//	{
	//		// TODO: To successfully use this example, you must perform the request
	//		// against a server that provides JSON data.
	//		// This example fails because the returned Content-Type is text/html and not application/json.
	////		web::http::client::http_client client("http://audiotools-audiotools.rhcloud.com/");
	//		web::http::client::http_client client("http://127.0.0.1:8080/widgets");
	//		//
	//		web::http::uri_builder builder(U("/widgets"));
	//
	//		return client.request(web::http::methods::GET, builder.to_string())
	//
	//			.then([](web::http::http_response response) -> pplx::task<web::json::value> {
	//
	//				if (response.status_code() == web::http::status_codes::OK) {
	//					return response.extract_json();
	//				}
	//
	//				// Handle error cases, for now return empty json value...
	//				return pplx::task_from_result(web::json::value());
	//			})
	//
	//			.then([](pplx::task<web::json::value> previousTask) {
	//
	//				try {
	//					const web::json::value& obj = previousTask.get();
	//
	////					auto arr = obj.as_array();
	//					for(auto iter = obj.as_object().cbegin(); iter != obj.as_object().cend(); ++iter)
	//					{
	//						// Make sure to get the value as const reference otherwise you will end up copying
	//						// the whole JSON value recursively which can be expensive if it is a nested
	// object.
	//						const std::string& name = iter->first;
	//						const web::json::value& v = iter->second;
	//
	//						// Perform actions here to process each string and value in the JSON object...
	//						std::cout << "String: " << name << ", Value: " << obj.as_string() << std::endl;
	//					}
	//
	//					// Perform actions here to process the JSON value...
	//				}
	//				catch (const web::http::http_exception& e) {
	//					// Print error.
	//					std::wostringstream ss;
	//					ss << e.what() << std::endl;
	//					std::wcout << ss.str();
	//				}
	//			});
	//
	//		/* Output:
	//		 Content-Type must be application/json to extract (is: text/html)
	//		 */
	//	}

	OnlineStoreMenu::OnlineStoreMenu(const ax::Rect& rect)
		: _font(0)
		, _font_bold("fonts/FreeSansBold.ttf")
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &OnlineStoreMenu::OnPaint);

		win->AddConnection(10, GetOnDoneDownloadingWidgetList());

		_store = std::shared_ptr<OnlineStore>(new OnlineStore(win));
		//		_store->DownloadWidgetData();

		// RequestJSONValueAsync();
	}

	void OnlineStoreMenu::OnDoneDownloadingWidgetList(const ax::event::EmptyMsg& msg)
	{
		ax::console::Print("Done downloading data.");
	}

	void OnlineStoreMenu::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.3));
		gc.DrawString(_font_bold, "No implemented yet.", ax::Point(15, 20));

		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}
}
}
