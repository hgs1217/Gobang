
let blackSelection,
    whiteSelection

$.extend({
    post: function(url, args){
        let body = $(document.body),
            form = $("<form method='post'></form>"),
            input
        form.attr({'action': url})
        $.each(args, function(key, value){
            input = $("<input type='hidden'>")
            input.attr({"name": key})
            input.val(value)
            form.append(input)
        })
        form.appendTo(body)
        form.submit()
        body.removeChild(form)
    }
})

function init() {
    document.oncontextmenu = function () {
        event.returnValue = false
    }
}

function start(e) {
    $.post('/board', {
        'p1Type': $('#black-selection').find("option:selected").val(),
        'p2Type': $('#white-selection').find("option:selected").val()
    })
}
