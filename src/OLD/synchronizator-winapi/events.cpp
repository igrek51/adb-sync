#include "app.h"
#include <commctrl.h>

void App::event_init(HWND *window){
    main_window = *window;
    //ustawienia
    Config::geti()->load_config();
    //log
    if(Config::geti()->log_enabled){
        IO::geti()->clear_log();
    }
    if(Config::geti()->history_enabled){
        historia->load();
    }
    //kontrolki
    IO::geti()->log("Tworzenie kontrolek...");
    //groupbox
    HWND handle = CreateWindowEx(0,WC_BUTTON,"Lista zadañ",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,0,0,0,0,main_window,(HMENU)0,*hInst,0);
    Controls::geti()->controls.push_back(new Control(handle, "groupbox1"));
    Controls::geti()->create_button("Szukaj", 0, 0, 0, 0, "szukaj");
    Controls::geti()->create_button("2 foldery", 0, 0, 0, 0, "2foldery");
    Controls::geti()->create_button("Porównaj", 0, 0, 0, 0, "porownaj_pliki");
    Controls::geti()->create_button("Usuñ", 0, 0, 0, 0, "usun");
    Controls::geti()->create_button("Odwróæ", 0, 0, 0, 0, "odwroc");
    Controls::geti()->create_button("Wykonaj 1", 0, 0, 0, 0, "wykonaj1");
    Controls::geti()->create_button("Wykonaj", 0, 0, 0, 0, "wykonaj");
    //multiline centered static
    handle = CreateWindowEx(0,WC_EDIT,"",WS_CHILD|WS_VISIBLE|ES_CENTER|ES_MULTILINE|ES_READONLY,0,0,0,0,main_window,0,*hInst,0);
    Controls::geti()->controls.push_back(new Control(handle, "info"));
    //listbox
    handle = CreateWindowEx(WS_EX_CLIENTEDGE,WC_LISTBOX,"",WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT|WS_HSCROLL|WS_VSCROLL|LBS_NOTIFY,0,0,0,0,main_window,0,*hInst,0);
    Controls::geti()->controls.push_back(new Control(handle, "listbox"));
    //progress bar
    handle = CreateWindowEx(0,PROGRESS_CLASS,NULL,WS_CHILD|WS_VISIBLE,0,0,0,0,main_window,(HMENU)200,*hInst,0);
    Controls::geti()->controls.push_back(new Control(handle, "progress_bar"));
    //czcionki
    controls_fonts_set();
    //subclassing
    IO::geti()->log("Subclassing...");
    for(unsigned int i=0; i<Controls::geti()->controls.size(); i++){
        subclass(Controls::geti()->controls.at(i));
    }
    //ustawiona pozycja, rozmiar okna
    if(Config::geti()->save_wnd_pos==1 && Config::geti()->save_wnd_size==1){
        SetWindowPos(main_window,HWND_TOP,Config::geti()->wnd_pos_x,Config::geti()->wnd_pos_y,Config::geti()->window_w,Config::geti()->window_h,0);
    }else if(Config::geti()->save_wnd_pos==1){
        SetWindowPos(main_window,HWND_TOP,Config::geti()->wnd_pos_x,Config::geti()->wnd_pos_y,0,0,SWP_NOSIZE);
    }else if(Config::geti()->save_wnd_size==1){
        SetWindowPos(main_window,HWND_TOP,0,0,Config::geti()->window_w,Config::geti()->window_h,SWP_NOMOVE);
    }
    event_resize();
    //progress bar
	SendMessage(Controls::geti()->find("progress_bar"),PBM_SETRANGE,0,(LPARAM)MAKELONG(0,10000));
	set_progress(0);
    //tytu³
	SetWindowText(main_window, (Config::geti()->program_name+" v"+Config::geti()->version).c_str());
    IO::geti()->echo("Naciœnij przycisk \"Szukaj\", aby wyszukaæ elementy synchronizacji.");
}

void App::event_button(WPARAM wParam, LPARAM lParam){
    //zmieniony listbox
	if(lParam==(LPARAM)Controls::geti()->find("listbox")&&HIWORD(wParam)==LBN_SELCHANGE){
		//listbox_clicked();
        return;
	}
    string name = "";
    if(wParam>=1 && wParam<=Controls::geti()->controls.size()){
        name = Controls::geti()->get_button_name(wParam);
    }
    if(name.length()==0) return;
	//przyciski
	if(name=="szukaj"){ //szukaj
		filesearch_start();
	}else if(name=="2foldery"){ //otwórz foldery
		otworz_foldery();
	}else if(name=="usun"){ //usuñ
		pb_usun();
	}else if(name=="wykonaj1"){ //wykonaj 1
		wykonaj_1();
	}else if(name=="wykonaj"){ //wykonaj wszystko
		wykonaj_wszystko();
	}else if(name=="porownaj_pliki"){ //otwórz pliki
		otworz_pliki();
	}else if(name=="odwroc"){ //odwróæ
		pb_odwroc();
	}else{
        IO::geti()->error("Zdarzenie nie zosta³o obs³u¿one: "+name);
    }
}

void App::event_resize(){
    IO::geti()->log("Resize okna - Odœwie¿anie uk³adu kontrolek...");
    if(!Controls::geti()->exists("info")) return;
    RECT wnd_rect;
	GetClientRect(main_window, &wnd_rect);
    int w = wnd_rect.right-wnd_rect.left;
	int h = wnd_rect.bottom-wnd_rect.top;
	Config::geti()->window_w = w;
    Config::geti()->window_h = h;
	text_vcenter();
    Controls::geti()->resize("szukaj",20+(w-40)*0/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("2foldery",20+(w-40)*1/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("porownaj_pliki",20+(w-40)*2/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("usun",20+(w-40)*3/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("odwroc",20+(w-40)*4/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("wykonaj1",20+(w-40)*5/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("wykonaj",20+(w-40)*6/7,h-50,(w-40)/7,30);
    Controls::geti()->resize("progress_bar",10,52,w-20,15);
    Controls::geti()->resize("listbox",20,90,w-40,h-150);
    Controls::geti()->resize("groupbox1",10,70,w-20,h-80);
}

void App::event_move(){
	RECT wnd_rect;
	GetWindowRect(main_window, &wnd_rect);
	Config::geti()->wnd_pos_x = wnd_rect.left;
	Config::geti()->wnd_pos_y = wnd_rect.top;
	Config::geti()->window_w = wnd_rect.right-wnd_rect.left;
	Config::geti()->window_h = wnd_rect.bottom-wnd_rect.top;
}

