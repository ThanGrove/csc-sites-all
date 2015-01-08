(function($){
    $(window).load(function(){
        $(".add-scroll").mCustomScrollbar();
        if($('#m-3223').find('a').length > 0) { $('#m-3223').find('a')[0].target = '_blank'; }
        
        //$("#calendar-link").fancybox({
          //ajax : {
              //type	: "GET"
          //}
        //});

        //$('.csc-directors-wrapper').on('click', function(){
          //$(this).find('a').click();
        //});
        
        // Hide Google Map link if Place is TBA
        var pf = $('.pane-node-field-location');
        if(pf.length > 0) {
	        var pval = pf.find('.adr .fn').text();
	        if(pval && pval == 'TBA') {
	        	$('.map-link .location.map-link').hide();
	        }
	      }

    });          
})(jQuery);
