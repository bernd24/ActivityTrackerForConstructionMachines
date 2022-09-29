using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;
using UIAutomate.Models;

namespace Server.Controllers
{
    public class SUSTestController : Controller
    {
        private readonly SUSTestContext _context;

        public SUSTestController(SUSTestContext context)
        {
            _context = context;
        }

        // GET: SUSTest
        public async Task<IActionResult> Formular()
        {
              return _context.SUSTest != null ? 
                          View(await _context.SUSTest.ToListAsync()) :
                          Problem("Entity set 'SUSTestContext.SUSTest'  is null.");
        }


        public async Task<IActionResult> Introduction()
        {
              return _context.SUSTest != null ? 
                          View(await _context.SUSTest.ToListAsync()) :
                          Problem("Entity set 'SUSTestContext.SUSTest'  is null.");
        }

        // GET: SUSTest/Details/5
        public async Task<IActionResult> Details(int? id)
        {
            if (id == null || _context.SUSTest == null)
            {
                return NotFound();
            }

            var sUSTest = await _context.SUSTest
                .FirstOrDefaultAsync(m => m.Id == id);
            if (sUSTest == null)
            {
                return NotFound();
            }

            return View(sUSTest);
        }

        // GET: SUSTest/Create
        public IActionResult Create()
        {
            return View();
        }

        // POST: SUSTest/Create
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("Id,Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree")] SUSTest sUSTest)
        {
            if (ModelState.IsValid)
            {
                _context.Add(sUSTest);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            return View(sUSTest);
        }

        // GET: SUSTest/Edit/5    duplicatet
        public async Task<IActionResult> SUSResults()
        {
            // if (id == null || _context.SUSTest == null)
            // {
            //     return NotFound();
            // }

            IEnumerable<SUSTest> sUSTest = await _context.SUSTest.ToListAsync();
            if (sUSTest == null)
            {
                return NotFound();
            }
            return View(sUSTest);
        }


        // GET: SUSTest/Edit/5
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> CreateForm(IFormCollection form)
        {
            if (form == null || _context.SUSTest == null)
            {
                return NotFound();
            }

            var data = new List<SUSTest>();
            var temp_Ids = form["item.id"];
            var temp_Questions = form["item.Question"];
            var temp_SDisagrees = form["item.StronglyDisagree"];
            var temp_Disagrees = form["item.Disagree"];
            var temp_Neutrals = form["item.Neutral"];
            var temp_Agrees = form["item.Agree"];
            var temp_SAgrees = form["item.StronglyAgree"];

            for (int i = 0; i < form["item.id"].Count(); i++)
            {
                var erg = _context.Find<SUSTest>(Convert.ToInt32(temp_Ids[i]));
                erg.Question = temp_Questions[i];
                erg.StronglyDisagree = ConvertToBool(temp_SDisagrees[i]);
                erg.Disagree = ConvertToBool(temp_Disagrees[i]);
                erg.Neutral = ConvertToBool(temp_Neutrals[i]);
                erg.Agree = ConvertToBool(temp_Agrees[i]);
                erg.StronglyAgree = ConvertToBool(temp_SAgrees[i]);
                if (ModelState.IsValid)
                {
                    _context.Update(erg);
                    await _context.SaveChangesAsync();
                }
            }
            return _context.SUSTest != null ? 
            View(await _context.SUSTest.ToListAsync()) :
            Problem("Entity set 'SUSTestContext.SUSTest'  is null.");
            // return View("Index");
        }

        private bool ConvertToBool(string temp){
            bool erg = false;
            if (temp.Equals("true"))
            {
                erg = true;
            }else if(temp.Equals("false")){
                erg= false;
            }
            return erg;
        }
        // GET: SUSTest/Edit/5

        // public async Task<IActionResult> Edit(int? id)
        // {
        //     if (id == null || _context.SUSTest == null)
        //     {
        //         return NotFound();
        //     }

        //     var sUSTest = await _context.SUSTest.FindAsync(id);
        //     if (sUSTest == null)
        //     {
        //         return NotFound();
        //     }
        //     return View(sUSTest);
        // }

        // POST: SUSTest/Edit/5
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        // [HttpPost]
        // [ValidateAntiForgeryToken]
        // public async Task<IActionResult> Edit(int id, [Bind("Id,Question,StronglyDisagree,Disagree,Neutral,Agree,StronglyAgree")] SUSTest sUSTest)
        // {
        //     if (id != sUSTest.Id)
        //     {
        //         return NotFound();
        //     }

        //     if (ModelState.IsValid)
        //     {
        //         try
        //         {
        //             _context.Update(sUSTest);
        //             await _context.SaveChangesAsync();
        //         }
        //         catch (DbUpdateConcurrencyException)
        //         {
        //             if (!SUSTestExists(sUSTest.Id))
        //             {
        //                 return NotFound();
        //             }
        //             else
        //             {
        //                 throw;
        //             }
        //         }
        //         return RedirectToAction(nameof(Index));
        //     }
        //     return View(sUSTest);
        // }

        // GET: SUSTest/Delete/5
        public async Task<IActionResult> Delete(int? id)
        {
            if (id == null || _context.SUSTest == null)
            {
                return NotFound();
            }

            var sUSTest = await _context.SUSTest
                .FirstOrDefaultAsync(m => m.Id == id);
            if (sUSTest == null)
            {
                return NotFound();
            }

            return View(sUSTest);
        }

        // POST: SUSTest/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id)
        {
            if (_context.SUSTest == null)
            {
                return Problem("Entity set 'SUSTestContext.SUSTest'  is null.");
            }
            var sUSTest = await _context.SUSTest.FindAsync(id);
            if (sUSTest != null)
            {
                _context.SUSTest.Remove(sUSTest);
            }
            
            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool SUSTestExists(int id)
        {
          return (_context.SUSTest?.Any(e => e.Id == id)).GetValueOrDefault();
        }
    }
}
