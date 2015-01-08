(function($){
    $(window).load(function(){
        $(".add-scroll").mCustomScrollbar();
        if($('#m-3223').find('a').length > 0) { $('#m-3223').find('a')[0].target = '_blank'; }
        
        // Hide Google Map link if Place is TBA
        var pf = $('.pane-node-field-location, .views-field-field-location');
        pf.each(function() {
	        var pval = $(this).find('.adr .fn').text();
	        if(pval && pval == 'TBA' || pval == '') {
	        	$(this).find('.map-link .location.map-link').hide();
	        }
	      });
	      // Hide map link if no coordinates/address given
	      $('.location.map-link').each(function() {
	      	var a = $(this).find('a');
	      	if(a.attr('href') == 'http://maps.google.com?q=%2C+%2C+%2C+%2C+us') {
	      		$(this).hide();
	      	}
	      });
	      // if field location empty, add TBA
	      $('.views-field-field-location').each(function() {
	      	var cnt = $(this).find('.field-content').html();
	      	if(cnt == '') {
	      		$(this).find('.field-content').text('TBA');
	      	}
	      });

    });          
})(jQuery);
