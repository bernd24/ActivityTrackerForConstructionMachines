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
        public async Task<IActionResult> Edit(IFormCollection list)
        {
            if (list == null || _context.SUSTest == null)
            {
                return NotFound();
            }

            var data = new List<SUSTest>();

            // foreach( var item in list){
            // for (int i = 0; i < list.Count(); i++)
            // {
            foreach (var item in list)
            {
                var erg = new SUSTest();
                for (int i = 0; i < item.Value.Count(); i++)
                {
                    erg.Id = Convert.ToInt32(item.Value[1]);

                    erg.Question = item.Value;
                    erg.StronglyDisagree = ConvertToBool(list.FirstOrDefault().Value[3]);
                    erg.Disagree = ConvertToBool(list.FirstOrDefault().Value[4]);
                    erg.Neutral = ConvertToBool(list.FirstOrDefault().Value[5]);
                    erg.Agree =  ConvertToBool(list.FirstOrDefault().Value[6]);
                    erg.StronglyAgree = ConvertToBool(list.FirstOrDefault().Value[7]);
                }
               
                data.Add(erg);
            }

            // var data = list as IEnumerable<SUSTest>;
            if (ModelState.IsValid)
            {
                _context.UpdateRange(data);
                await _context.SaveChangesAsync();
            }
            return View();
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

        public async Task<IActionResult> Edit(int? id)
        {
            if (id == null || _context.SUSTest == null)
            {
                return NotFound();
            }

            var sUSTest = await _context.SUSTest.FindAsync(id);
            if (sUSTest == null)
            {
                return NotFound();
            }
            return View(sUSTest);
        }

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
