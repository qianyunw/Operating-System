// 禁止IE6.0
var browser=navigator.appName
var b_version=navigator.appVersion
var version=b_version.split(";");
var trim_Version=version[1].replace(/[ ]/g,"");
if(browser=="Microsoft Internet Explorer" )
{ 
  if(trim_Version=="MSIE6.0")
    {window.location.href("/Empty/ieerro");}
//  else if(trim_Version=="MSIE8.0")
      //alert("亲，既然您的浏览器已经升级到了IE8.0，为什么不尝试升级到IE10.0呢？\n这样你将拥有更好的浏览效果");
      //强制跳转
  //    window.location.href("http://www.baidu.com");
}

//主页的UL 下拉
function test()
{
  alert("暂未开放敬请期待");
}
//显示列表
$('#select-selected').mouseover(function(){
  $('#select-list').hide().fadeIn();
  //切换背景
});
//隐藏列表
$('#select-con').mouseleave(function(){
  $('#select-list').hide();
});
$('#select-list li').click(function(){
  $('#select-list').hide().fadeOut();
  var sleval =$(this).text() ;//$(this).attr('listfor');
  $('#select-selected').html(sleval);
  //设置隐藏域
  var stval = $(this).attr('listfor');
  //设置隐藏域的 value
  $('#st').val(stval);
});
//这只默认的选项
var setDefSelect = function(){  // //查找 UL下所有LI
  var list = $('#select-list li');
  list.each(function(e){
    var liSet = $(this).attr('selected')
    if(liSet=='selected'){
      
      var sleval =$(this).text();
      //切换当前选中的 项目
      $('#select-selected').html(sleval);
      //设置隐藏域
      var stval = $(this).attr('listfor');
      //设置隐藏域的 value
      $('#st').val(stval);
    }
  });
}
setDefSelect();

//   if(!+[1,]){
//     //对于IE调整 下拉框的位置
//     $('#select-con').css('margin-top',0);
//     $('#select-con').css('border-right','none');
// }
$('#sub-btn').click(function(){
 var keyw = $('#search-text').attr('value');
 keyw=keyw.trim();
 if (keyw.length <2){alert("关键字不能低于2个!");return false;}
  return true;
});

///////////////////////////////////返回顶部的 js段

var VarSrcCon =$('#idcon');
var VarBackTop = $('#back-top');
//只执行 top按钮
var VarTop = $('#id-backtop');
//先隐藏
VarBackTop.css('display','none');
VarTop.css('display','none');
//函数用于设置 显示的位置
function TraceBackPos(){
	WCon = VarSrcCon.outerWidth();//容器的宽度
	Conleftpos = VarSrcCon.offset().left;//容器的左侧的坐标
	leftPos = Conleftpos+WCon;//返回位置的左侧坐标
	leftPos+=2;
	szPos = leftPos+"px";
	VarBackTop.css('left',szPos);
	VarBackTop.show();

}
//窗口加载的时候执行
$(document).ready(function(){
	TraceBackPos();
});
//窗口的大小发生变化的时候，调整
$(window).resize(function(){
    TraceBackPos();
  });
//根据滚动条的位置 显示
var CriticalHeight = 400;
  $(window).scroll(function(){
    iet = document.documentElement.scrollTop;
	other= $(document).scrollTop();
	vstop = iet|other;
	if(vstop>CriticalHeight){
		VarTop.slideDown(300);
	}else{
		VarTop.slideUp(300);
	}
  });
var currentPosition,timer; 
var bs = 400;
function GoTop()
{ 
	//timer=setInterval("runToTop()",10); 
	$('body,html').animate({scrollTop:0},200);
} 

//不使用的函数
function runToTop()
{ 
	currentPosition=document.documentElement.scrollTop || document.body.scrollTop; 
	currentPosition-=bs; 
	if(currentPosition>0) 
	{ 
		window.scrollTo(0,currentPosition); 
	} 
	else 
	{ 
		window.scrollTo(0,0); 
		clearInterval(timer); 
	} 
}

//刷新
function flush()
{
	window.top.location.reload();
}
//前往留言板
function goboard()
{
	window.top.location.href="/say";
}
